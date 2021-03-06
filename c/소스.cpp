// project.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "stdlib.h"
#include "windows.h"
#include "time.h"
#include "conio.h" //_kbhit(),getch()를 사용하기위한 헤더파일

void elpTime();
void pTime();
void keyListener();
void setAlarm();
void writeData();
void readData();

typedef struct SaveTime {  //구조체단위로 파일입출력함
	int date;
	int elaTime;
}saveTime;

int c = 0; //clock()로 받은 값을 저장
int m = 0, h = 0; // 경괴시간 분, 시
int state = NULL; // 키입력 상태
int endProgram = 1; //프로그램 종료

int alarm = 3600;// 초로 환산한 알람시간



#define ESC 27

int main()
{
	system("mode con:cols=20 lines=5");// con:cols=50 : 가로 길 lines=20 : 세로 길

	while (endProgram) {
		system("cls");
		keyListener();
		elpTime();
		pTime();
		printf("0 도움말, ESC 종료.\n");
		Sleep(50);

	}

	writeData(); //시간값 저장
	return 0;
}

void elpTime() {		//elapse time 경과시간
	c = clock() / 100; //clock()는 1/1000초를 반환하므로 1초로 단위 변경
	m = (c / 60) % 60;
	h = c / 3600;
	if ((c % alarm) == 0) { //알람시간이 되면 비프음 출력
		printf("아무키 입력시 종료\n");
		for (int i = 0; i < 5; i++){
			Beep(262, 100);
			Beep(330, 100);
			Beep(392, 100);
			Beep(524, 100);
			if (_kbhit() != 0) {
				break;
			}
		}
	}

	printf("경과시간 %.2d:%.2d\n", h, m);

}

void pTime() {	//present time 현재시간
	struct tm *t;
	time_t timer;
	timer = time(NULL);
	t = localtime(&timer);
	printf("현재시간 %.2d:%.2d\n", t->tm_hour, t->tm_min);
}

void keyListener() {
	if (_kbhit() != 0) //키보드 버퍼값이 있으면 반복문을 빠져나옴
	{
		state = getch();
		switch (state) {
		case ESC:
			printf("종료합니다.\n");
			endProgram = 0;
			break;
		case '0':
			system("mode con:cols=50 lines=20");
			printf("1. 알람시간 설정\n2. 뽀모도르 시간관리\n3. 컴퓨터 사용량 확인\nESC. 이전\n");
			state = getch();
			switch (state) {
			case ESC:
				break;
			case '1':
				setAlarm();
				break;
			case '2':
				//pomodoro();
				break;
			case '3':
				readData();
				break;
			}
			system("mode con:cols=20 lines=5");
			break;
		}
	}

}

void setAlarm() {
	int alarmH = 0, alarmM = 01; // 알람시간 시, 분
	system("cls");
	printf("알람 설정 메뉴입니다.(기본 1시간마다 알람)\n");
	printf("----------------------\n");
	while (1){
		while (1){  //알람 시 설정
			printf("시간을 설정해 주십시오( 0 ~ 24 )\n");
			scanf("%d", &alarmH);
			if (alarmH > 24) {
				printf("24시를 넘을 수 없습니다.\n");
				continue;
			}
			break;
		}
		while (1){ // 알람 분 설정
			printf("분을 설정해 주십시오( 0 ~ 59 )\n");
			scanf("%d", &alarmM);
			if (alarmH > 59) {
				printf("59분를 넘을 수 없습니다.\n");
				continue;
			}
			break;
		}
		alarm = (alarmH * 3600) + (alarmM * 60); //시, 분을 초단위로 변환 하여 alarm에 저장
		if (alarm == 0) {  //0시 0분일 때를 방지
			printf("시간을 다시설정 하세요.\n");
			continue;
		}
		break;
	}

}

void writeData() { //시간 기록을 파일 입출력으로 저장
	FILE * fp;
	
	if ((fp = fopen("Log.txt", "a")) == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return;
	}
	saveTime save;
	save.date = time(NULL);
	save.elaTime = clock();
	fwrite(&save, sizeof(saveTime), 1, fp);
	Sleep(500);
	fclose(fp);
}

void readData() { //시간기록을 log.txt에서 읽어 들임
	FILE * fp;
	
	if ((fp = fopen("Log.txt", "r")) == NULL) {
		printf("파일을 열 수 없습니다.\n");
	}

	saveTime read;
	struct tm * t;
	time_t timmer;

	while (fread(&read, sizeof(saveTime), 1, fp) != NULL){
		timmer = (time_t)read.date;
		t = localtime(&timmer);
		printf("%d %d\n", t->tm_hour, read.elaTime);
	}
	printf("아무키나 누르시오\n");
	getch();

}