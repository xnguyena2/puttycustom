// ConvertPem2PPK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
extern "C" {
#include "putty.h"
#include "ssh.h"
}

#include <iostream>
#include <assert.h>

int main(int argc, char *argv[])
{
	Filename *infilename = NULL, *outfilename = NULL;
	int sshver = 0;
	struct ssh2_userkey *ssh2key = NULL;
	int intype = SSH_KEYTYPE_UNOPENABLE;
	const char *error = NULL;
	int ret;

	char *infile = argv[1];
	char *outfile = argv[2];


	if (infile && outfile) {
		infilename = filename_from_str(infile);
		outfilename = filename_from_str(outfile);

		intype = key_type(infilename);

		switch (intype) {
		case SSH_KEYTYPE_UNOPENABLE:
		case SSH_KEYTYPE_UNKNOWN:
			fprintf(stderr, "puttygen: unable to load file `%s': %s\n",
				infile, key_type_to_str(intype));
			return 1;

		case SSH_KEYTYPE_SSH1:
		case SSH_KEYTYPE_SSH1_PUBLIC:
			if (sshver == 2) {
				fprintf(stderr, "puttygen: conversion from SSH-1 to SSH-2 keys"
					" not supported\n");
				return 1;
			}
			sshver = 1;
			break;

		case SSH_KEYTYPE_SSH2:
		case SSH_KEYTYPE_SSH2_PUBLIC_RFC4716:
		case SSH_KEYTYPE_SSH2_PUBLIC_OPENSSH:
		case SSH_KEYTYPE_OPENSSH_PEM:
		case SSH_KEYTYPE_OPENSSH_NEW:
		case SSH_KEYTYPE_SSHCOM:
			if (sshver == 1) {
				fprintf(stderr, "puttygen: conversion from SSH-2 to SSH-1 keys"
					" not supported\n");
				return 1;
			}
			sshver = 2;
			ssh2key = import_ssh2(infilename, intype, NULL, &error);
			if (ssh2key) {
				if (ssh2key != SSH2_WRONG_PASSPHRASE)
					error = NULL;
				else
					error = "wrong passphrase";
			}
			else if (!error)
				error = "unknown error";

			assert(ssh2key);
			ret = ssh2_save_userkey(outfilename, ssh2key, NULL);
			if (!ret) {
				fprintf(stderr, "puttygen: unable to save SSH-2 private key\n");
				return 1;
			}
			break;

		case SSH_KEYTYPE_OPENSSH_AUTO:
		default:
			assert(0 && "Should never see these types on an input file");
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
