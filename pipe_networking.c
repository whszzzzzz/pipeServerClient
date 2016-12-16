#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipe_networking.h"

int server_handshake(int * from_client){
	int status;
	int to_client;
	char received[MESSAGE_BUFFER_SIZE];
	char out[MESSAGE_BUFFER_SIZE];

	status = mkfifo("mario", 0644);
	*from_client = open("mario", O_RDONLY, 0644);

	printf("[SERVER] Waiting for connection: \n");
	read(*from_client, received, sizeof(received));
	printf("[SEVER] Incoming connection from %s\n", received );
	remove("mario");

	//BREAK UP RIGHT HERE

	to_client = open(received, O_WRONLY, 0644);

	strcpy(out,"Welcome");
	write(to_client, out, sizeof(out));

	printf("to_client: %d\n", to_client);
	return to_client;
}

int client_handshake(int * to_server){
	int status;
	char pid[MESSAGE_BUFFER_SIZE];
	char received[MESSAGE_BUFFER_SIZE];
	char out[MESSAGE_BUFFER_SIZE];
	int from_server;

	sprintf(pid, "%d" ,getpid());
	status = mkfifo(pid, 0644);

	*to_server = open("mario", O_WRONLY, 0644);
	write(*to_server, pid, sizeof(out));

	from_server = open(pid, O_RDONLY, 0644);
	read(from_server, received, sizeof(received));
	printf("[CLIENT] received: %s\n", received);

	remove(pid);
	strcpy(out, "Hi");

	write(*to_server, out, sizeof(out));

	printf("from_server: %d\n", from_server);
	return from_server;
}
