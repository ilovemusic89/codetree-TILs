#include <stdio.h>
#include <iostream>
#include <unordered_map>
using namespace std;
//#define _crt_secure_no_warnings
#pragma warning(disable: 4996) 


struct BOX {
	int idx;
	int weight;
	int belt;
	BOX* prev;
	BOX* next;

	BOX() {
		idx = -1, weight = -1, belt = -1;
		prev = NULL, next = NULL;
	}

};

struct BELT {
	bool is_broken;
	BOX* head;
	BOX* tail;

	BELT() {
		is_broken = false;
		head = new BOX();
		tail = new BOX();
		head->next = tail, tail->prev = head;
		head->prev = NULL, tail->next = NULL;
	}
};

int q, cmd, n, m;
BOX box[100000];
BELT belt[10];
unordered_map<int, BOX*> box_map;

bool empty(int belt_num) {
	return (belt[belt_num].head->next == belt[belt_num].tail);
}

void push_back(int belt_num, BOX* item) {  // 벨트번호, 상자ID 를 받음
	box_map[item->idx] = item;  // box_map 의 key 는 상자ID
	BOX* prev = belt[belt_num].tail->prev;   // tail 이전
	BOX* next = belt[belt_num].tail;   // tail

	item->prev = prev; // ITEM PRE
	item->next = next; // ITEM NEXT 
	prev->next = item;  // 기존 값 TAIL PRE NEXT
	next->prev = item; // 기존 값 TAIL PRE
}

void pop_front(int belt_num) {
	if (empty(belt_num)) {
		return;
	}

	BOX* item = belt[belt_num].head->next;
	box_map.erase(item->idx);
	BOX* prev = belt[belt_num].head;
	BOX* next = belt[belt_num].head->next->next;

	prev->next = next;
	next->prev = prev;
	item->prev= NULL;
	item->next = NULL;
}

void q_100() {
	scanf("%d %d", &n, &m);  // 상자수, 벨트수
	int id[100000], w[100000];
	for (int i = 0; i < n; ++i) { // 상자수만큼
		scanf("%d", &id[i]);
	}
	for (int i = 0; i < n; ++i) { // 무게도 상자수 만큼
		scanf("%d", &w[i]);
	}
	int pos = 0;
	for (int belt_num = 0; belt_num < m; ++belt_num) {
		for (int i = 0; i < n / m; ++i) {
			box[pos].idx = id[pos];
			box[pos].weight = w[pos];
			box[pos].belt = belt_num;
			push_back(belt_num, &box[pos]);
			++pos;
		}
	}
}

int q_200() {
	int ret = 0;
	int w;
	scanf("%d", &w);
	for (int i = 0; i < m; ++i) {
		if (empty(i) || belt[i].is_broken) {
			continue;
		}
		BOX* front = belt[i].head->next;
		if (front->weight <= w) {
			ret += front->weight;
			pop_front(i);
		}
		else {
			pop_front(i);
			push_back(i, front);
		}
	}
	return ret;
}

int q_300() {
	int id;
	scanf("%d", &id);
	if (box_map.find(id) == box_map.end()) {
		return -1;
	}

	BOX* item = box_map[id];
	BOX* prev = item->prev;
	BOX* next = item->next;

	prev->next = next;
	next->prev = prev;

	item->prev = NULL;
	item->next = NULL;

	box_map.erase(id);
	return id;
}

int q_400() {
	int id;
	scanf("%d", &id);
	if (box_map.find(id) == box_map.end()) {
		return -1;
	}

	BOX* item = box_map[id];
	BOX* prev = item->prev;
	BOX* begin = belt[item->belt].head->next;
	BOX* end = belt[item->belt].tail->prev;

	if (item != begin) {
		item->prev = belt[item->belt].head;
		belt[item->belt].head->next = item;

		begin->prev = end;
		end->next = begin;

		prev->next = belt[item->belt].tail;
		belt[item->belt].tail->prev = prev;
	}
	return item->belt + 1;
}

int q_500() {
	int belt_num;
	scanf("%d", &belt_num);
	--belt_num;

	if (belt[belt_num].is_broken) {
		return -1;
	}

	belt[belt_num].is_broken = true;

	if (!empty(belt_num)) {
		int target = (belt_num + 1) % m;
		while (belt[target].is_broken) {
			target = (target + 1) % m;
		}
		BOX* prev = belt[target].tail->prev;
		BOX* begin = belt[belt_num].head->next;
		BOX* end = belt[belt_num].tail->prev;

		for (auto it = begin; it != belt[belt_num].tail; it = it->next) {
			it->belt = target;
		}

		prev->next = begin;
		begin->prev = prev;

		end->next = begin;
		begin->prev = prev;

		end->next = belt[target].tail;
		belt[target].tail->prev = end;

		belt[belt_num].head->next = belt[belt_num].tail;
		belt[belt_num].tail->prev = belt[belt_num].head;
	}
	return belt_num + 1;
}


int main()
{
	scanf("%d", &q);

	while (q--) {
		scanf("%d", &cmd);

		switch (cmd) {
		case 100:
			q_100();
			printf("test!\n");
			break;
		case 200:
			//printf("%d\n", q_200());
			cout << q_200() << endl;
			break;
		case 300:
			cout << q_300() << endl;
			//printf("%d\n", q_300());
			break;
		case 400:
			cout << q_400() << endl;
			//printf("%d\n", q_400());
			break;
		case 500:
			cout << q_500() << endl;
			//printf("%d\n", q_500());
			break;
		}
	}

	return 0;
}


/*

7
100 12 3 10 12 20 15 14 19 22 25 16 17 21 18 30 30 20 20 10 18 17 15 25 11 14 17
200 25
300 22
300 999
400 14
400 18

*/