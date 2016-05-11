
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define buffer_size 25
#define rd 0
#define wrt 1

void getinfo(char *buffer){
		  struct timeval tv;
		  time_t curtime;
		  gettimeofday(&tv, NULL); 
		  curtime=tv.tv_sec;

		  strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
		}

int main()
{
	
	
	int mypipe1[2];
	int mypipe2[2];
	int mypipe3[2];
	
	char rmessage[buffer_size]="";
	
	pid_t pid1, pid2;
	
	pipe(mypipe1);
	pipe(mypipe2);
	pipe(mypipe3);

	
	
	pid1 = fork();
	
	if (pid1 == 0) { 	/*first child process*/  
	wait(NULL);
	close(mypipe1[wrt]);
	read(mypipe1[rd], rmessage, buffer_size);
	
	printf("Child 1: Time at Parent is %s\n", rmessage);
	sleep(3);
	char date[30];
	getinfo(date);
	
	close(mypipe2[rd]);
	write(mypipe2[wrt], date, strlen(date));

	
	
	}
	else { 
		
	  pid2 = fork();
	  
	  if(pid2==0){ /* second child process */
		 
			wait(NULL);
			close(mypipe2[wrt]);
			read(mypipe2[rd], rmessage, buffer_size);
			printf("Child 2: Time at Child 1 is %s\n", rmessage);
			sleep(3);
			char date[30];
			getinfo(date);
	
			close(mypipe3[rd]);
			write(mypipe3[wrt], date, strlen(date));
				 
	}
	else{ /*parent process */
		char date[30];
		getinfo(date);
		close(mypipe1[rd]);
		write(mypipe1[wrt], date, strlen(date));
		
	wait(NULL);
	wait(NULL);
		close(mypipe3[wrt]);
			read(mypipe3[rd], rmessage, buffer_size);
			printf("Parent: Time at Child 2 is %s\n", rmessage);
			printf("Parent: Sent a kill signal to Child 1 with PID %d\n", pid1);
			kill(pid1, SIGKILL);
			printf("Child with PID %d says good bye...\n",pid1);
			printf("Parent: Sent a kill signal to Child 2 with PID %d\n", pid2);
			kill(pid2, SIGKILL);
			printf("Child with PID %d says good bye...\n",pid2);
		
		
	}
}
	return 0;
}
