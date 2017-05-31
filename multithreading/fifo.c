#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define STA_READY 0
#define STA_RUN 1
#define STA_END 2
#define NR_PC 10

#define SCHE_FIFO

struct PCBNode{
	char processName[20];
	int processID;
	int remainSecs;
	int ArriveTime;
	int state;
	struct PCBNode *Next;

};

struct PCBNode nodes[NR_PC];
struct PCBNode *head, *tail;

void initialize(){
	int i;
	for(i=0;i<NR_PC;i++){
		sprintf(nodes[i].processName,"proc_%d",i);
		nodes[i].processID = i;
		nodes[i].remainSecs = rand()%10 + 1; //at least 1
		nodes[i].ArriveTime = rand()%10;
	}
}

void setup_ready_queue(){
	int i,j;
	struct PCBNode node;

	for(i=0;i<NR_PC-1;i++){
		for(j=i+1;j<NR_PC;j++){
			if(nodes[i].ArriveTime > nodes[j].ArriveTime){
				memcpy(&node,&nodes[i],sizeof(struct PCBNode));
				memcpy(&nodes[i],&nodes[j],sizeof(struct PCBNode));
				memcpy(&nodes[j],&node,sizeof(struct PCBNode));
			}
		}
	}
	
	for(i=0;i<NR_PC-1;i++){
		nodes[i].Next = &nodes[i+1];
		nodes[i].state = STA_READY;
	}
	nodes[NR_PC-1].Next = NULL;
	nodes[NR_PC-1].state = STA_READY;
	head = &nodes[0];
	tail = &nodes[NR_PC-1];



}

void print_ready_queue(){
	struct PCBNode *p;
	printf("Ready queue: ");
	for(p = head;p!=NULL;p=p->Next){
		printf("%s[%d]",p->processName,p->remainSecs);
	}
	printf("\n");
}

void __schedule_FIFO(){
	printf("FIFO:run progress: %s\n",head->processName);
	head->state = STA_RUN;
	head->remainSecs = 0;
	head->state = STA_END;
	head = head->Next;
}

void __schedule(){
#ifdef SCHE_FIFO
	__schedule_FIFO();
#else
#endif
}

void schedule(){
	while(head != NULL && head ->state == STA_READY){
		print_ready_queue();
		__schedule();
	}
}

int main(){
	initialize();
	setup_ready_queue();
	schedule();
}
