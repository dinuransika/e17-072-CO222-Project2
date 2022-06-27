# include <stdio.h> 
# include <string.h>
# include<ctype.h> 
# include<stdlib.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x+1))


typedef struct node{
	char c[100];
	int count;
	struct node * next;
} node_t;
typedef struct alp{
	char c;
	int count;
} alp_t;
typedef struct word{
	char c[20];
	int count;
} word_t;

int read_from_file_c(FILE *inputfp[50], alp_t alphabet[36], int fc);
void print_all(node_t * n);
void string_copy(char a[100], char b[100]);
int count_all(node_t * n);
void sort(alp_t a[36], int l, alp_t b[10]);
void read_from_file_w(FILE * inputfp[50], int fc);
int word_freq(node_t *n, word_t w[100000]);
void sort_w(word_t a[], int l, word_t b[10]);
node_t *head = NULL;
void brick();
void move_up(int y);
void move_down(int y);
void move_right(int x);
void move_left(int x);
void bar(int len);
void chart(alp_t a[36], int l, int flag, int len);
int max_len(int L, word_t a[L]);
void chart_w(int l,word_t a[l],int flag, int len);
void reverse(node_t **a);
void swap(alp_t arr[], int firstIndex, int secondIndex);
int indexOfMax(alp_t arr[], int startIndex, int n) ;
void selectionSort(alp_t arr[], int n);
void chart1(alp_t a[36], int l, int flag, int len);
int count_char(alp_t a[36]);
void chart_2(int l,word_t a[l],int flag, int len);

int main(int argc,char** argv)
{	
	char filename[20];
	strcpy(filename, argv[0]);
	int c_flag = 0, w_flag=0, scale_flag = 0, length_u = 10, file_counter=0;
	FILE * inputfp[50];
	for(int i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strcmp(argv[i], "-c") == 0) c_flag = 1;
			if(strcmp(argv[i], "-w") == 0) w_flag = 1;
			if(strcmp(argv[i], "--scaled") == 0) scale_flag = 1;
			if(strcmp(argv[i], "-l") == 0){
				if(strcmp(argv[i+1], "0")==0) return 0;
				else{
					if(atoi(argv[i+1]) > 0){length_u = atoi(argv[i+1]);}
					else {
						if(atoi(argv[i+1]) < 0){printf("Invalid option(negative) for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", filename); return 0;}
						else {printf("Invalid option for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", filename); return 0;}
					}
				}		
			}
			else{
				if(strcmp(argv[i], "-c") != 0 && strcmp(argv[i], "-w") != 0 && strcmp(argv[i], "--scaled") != 0 && strcmp(argv[i], "-l") != 0){
					printf("Invalid option [%s]\nusage: ./freqv3 [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", argv[i]);
					return 0;
				}
			}
		}else{
			if(atoi(argv[i])<=0){
				inputfp[file_counter] = fopen(argv[i], "r");
				if(inputfp[file_counter] == NULL){printf("Cannot open one or more given files\n"); return 0;}
				file_counter ++;
			}
		}
	}
	if(c_flag == 1 && w_flag==1){
		printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", filename);
		return 0;	
	}else{
		if(file_counter==0){printf("No input files were given\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", filename); return 0;}
		else{
			if(c_flag==1){
				alp_t al[36];
				alp_t b[10];
				int count_a = read_from_file_c(inputfp, al, file_counter);
				if(count_a == 0){printf("No data to process\n"); return 0;}
				sort(al, 36, b);
				if(count_char(al)<length_u) length_u = count_char(al);
				chart1(b, length_u, !scale_flag, count_a);
			}else{
				word_t w[100000], w1[10];
				read_from_file_w(inputfp, file_counter);
				int Len = count_all(head);
				if(Len == 0){printf("No data to process\n"); return 0;}
				reverse(&head);
				int l=word_freq(head, w);
				sort_w(w, l, w1);
				if(l<length_u) length_u = l;
				chart_2(length_u, w1, !scale_flag, Len);
			}
		}
	}
	
	return 0;		 
}
void read_from_file_w(FILE * inputfp[50], int fc){
	char ch[100];
	int i = 0, tot = 0;
	for(int i=0; i<fc; i++){
		while( fscanf(inputfp[i], "%s", ch) != EOF ){
			node_t *p = (node_t*)malloc(sizeof(node_t));
			char t_c[100];
			string_copy(t_c, ch);
			if(strlen(t_c)!=0){
				string_copy(p->c, ch);
				p->count = 0;		
				//strcpy(p->c, ch);
				p->next = head;
				head = p;
			}		
		}
	fclose(inputfp[i]);
	}
}
int read_from_file_c(FILE *inputfp[50], alp_t alphabet[36], int fc){
	char ch; 
	int co = 0, flag = 0;
	int i=0;
	for(int i=0 ; i<36 ; i++){
		alphabet[i].count =0;
	}
	for(int x=0; x<fc; x++){
		while( (ch = fgetc(inputfp[x])) != EOF ){
			ch = tolower(ch);
			flag = 0;
			if(((ch>='a' && ch<='z') || (ch>='0' && ch<='9'))){
				for(int j=0; j<co; j++){
					if(alphabet[j].c==ch){
						alphabet[j].count++;
						flag=1;
						i++;
						break;
					}
				}
				if(flag==0){
					alphabet[co].c = ch;
					alphabet[co].count = 1; 
					co++;
					i++;
				}	
			}
		}
		fclose(inputfp[x]);
	}
	return i;
}
int count_char(alp_t a[36]){
	int c=0;
	for(int i=0; i<36; i++){
		if(a[i].c!=0) c++;
	}
	return c;
}
void print_all(node_t * n){
	while(n!=NULL){
		printf("%s\n", n->c);
		n = n->next;	
	}
	printf("\n");
}
void string_copy(char a[100], char b[100]){
	for(int i=0, j=0 ; b[i]!='\0' ; i++){
		if((b[i] <= 'z' && b[i] >= 'a') || (b[i] <= 'Z' && b[i] >= 'A') || (b[i] >= '0' && b[i] <= '9' )){
			b[i] = tolower(b[i]);
			a[j] = b[i];
			j++;		
		}	
	}
}
int count_all(node_t * n){
	int z=0;
	while(n!=NULL){
		if(strlen(n->c)!=0){
			z++;		
		}
		n = n->next;	
	}
	return z;
}
void swap(alp_t arr[], int firstIndex, int secondIndex) {   
    	int temp;
	char tempc;
    	temp = arr[firstIndex].count;
	tempc = arr[firstIndex].c;
    	arr[firstIndex].count = arr[secondIndex].count;
	arr[firstIndex].c = arr[secondIndex].c;
    	arr[secondIndex].count = temp;
	arr[secondIndex].c = tempc;
}
int indexOfMax(alp_t arr[], int startIndex, int n) {
    int maxValue = arr[startIndex].count;
    int maxIndex = startIndex;

    for(int i = maxIndex + 1; i < n; i++) {
        if(arr[i].count > maxValue) 
        {
            maxIndex = i;
            maxValue = arr[i].count;
        }
    } 
    return maxIndex;
}
void selectionSort(alp_t arr[], int n) {
    for(int i = 0; i < n; i++) 
    {
        int index = indexOfMax(arr, i, n);
        swap(arr, i, index);
    }
    
}

void sort(alp_t a[36], int l, alp_t b[10]){
	int m;
	char d;
	int max=0, maxindex=0;
	for(int i=0; i<36; i++){
		if(max<a[i].count){
			max = a[i].count;
			b[0].count = max;
			b[0].c = a[i].c;
			maxindex = i;		
		}
	}
	a[maxindex].count = 0;
	for(int i=0; i<l; i++){
		max = 0;
		for(int j=0; j<36; j++){
			if(max<a[j].count && (a[j].count<=b[i].count)){
				b[i+1].count = a[j].count;
				b[i+1].c = a[j].c;
				max = a[j].count;
				maxindex = j;
			}
		}
		a[maxindex].count = 0;
	}
}
void sort_w(word_t a[], int l, word_t b[10]){
	int m;
	char d[20];
	int max=0, maxindex=0;
	for(int i=0; i<l; i++){
		if(max<a[i].count){
			max = a[i].count;
			b[0].count = max;
			strcpy(b[0].c,a[i].c);
			maxindex = i;		
		}
	}
	a[maxindex].count=0;
	for(int i=0; i<50; i++){
		max = -1;
		for(int j=0; j<l; j++){
			if(max<a[j].count && a[j].count<=b[i].count){
				b[i+1].count = a[j].count;
				strcpy(b[i+1].c, a[j].c);
				max = a[j].count;
				maxindex = j;
			}
		}
		a[maxindex].count = 0;
	}
}
int word_freq(node_t *n, word_t w[100000]){
	int q=0, flag=0;
	while(n!=NULL){
		flag=0;
		for(int i=0; i<q; i++){
			if(strcmp(w[i].c, n->c)==0){
				w[i].count++;
				flag=1;
				
				break;
			}
		}
		if(flag==0 && strlen(n->c)!=0){
			strcpy(w[q].c, n->c);
			w[q].count = 1;
			q++;
		}
		n = n->next;
	}
	return q;
}
void reverse(node_t **a){
	node_t * prev = NULL;
	node_t * current = *a;
	node_t * next;
	while(current!=NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*a = prev;
}
void move_up(int y){
	printf("\033[%dA",y);
}
void move_down(int y){
	printf("\033[%dB", y); 
}
void move_right(int x){
	printf("\033[%dC", x); // Move right X column;
}
void move_left(int x){
	printf("\033[%dD", x);
}
void brick(){
	for(int i=0; i<3; i++){
		printf("\u2591");
		move_down(1);
		move_left(1);
	}
	move_up(3);
	move_right(1);
}
void bar(int len){
	for(int i=0; i<len; i++){
		brick();
	}
}
void tape(int len){
	for(int i=0; i<len; i++){
		printf("\u2591");
	}
}
void chart1(alp_t a[36], int l, int flag, int len){
	if(a[0].count==len){
		float fr=0;
		printf("\n");
		float fr1 = (float)a[0].count*100/len;
		printf("   \u2502");
		tape(80 - (((fr1/10)>=1) + 10));
		printf("\n");
		printf(" %c \u2502", a[0].c);
		tape(80 - (((fr1/10)>=1) + 10));
		printf("%.2f%c\n", fr1, '%');
		printf("   \u2502");
		tape(80 - (((fr1/10)>=1) + 10));
		printf("\n");
		printf("   \u2502\n");	
		printf("   \u2514");
		for(int j=0; j<76; ++j){
			printf("\u2500");
		}
		printf("\n");
		return ;
	}
	if(flag == 1){
		float fr=0;
		printf("\n");
		for(int i=0; i<4*l; i++){
			fr = (float)a[(i)/4].count*100/len;
			if(i%4 == 0 || i%4==2){
				printf("   \u2502");
				tape((int)(fr*0.7));
				printf("\n");
			}else{
				if(i%4==1){
					printf(" %c \u2502", a[i/4].c);
					tape((int)(fr*0.7));
					printf("%.2f%c\n", fr, '%');		
				}else{
					if(i%4==3){
						printf("   \u2502\n");
					}
				}
			}
		}
	}else{
		float fr=0;
		printf("\n");
		float fr1 = (float)a[0].count*100/len;
		printf("   \u2502");
		tape(80 - (((fr1/10)>=1) + 9));
		printf("\n");
		printf(" %c \u2502", a[0].c);
		tape(80 - (((fr1/10)>=1) + 9));
		printf("%.2f%c\n", fr1, '%');
		printf("   \u2502");
		tape(80 - (((fr1/10)>=1) + 9));
		printf("\n");
		printf("   \u2502\n");
		for(int i=4; i<4*l; i++){
			fr = (float)a[i/4].count*100/len;
			float fr2 = fr*70/fr1; 
			if(i%4 == 0 || i%4==2){
				printf("   \u2502");
				tape((int)(fr2));
				printf("\n");
			}else{
				if(i%4==1){
					printf(" %c \u2502", a[i/4].c);
					tape((int)(fr2));
					printf("%.2f%c\n", fr, '%');		
				}else{
					if(i%4==3){
						printf("   \u2502\n");
					}
				}
			}
		}
	}
	printf("   \u2514");
	for(int j=0; j<76; ++j){
		printf("\u2500");
	}
	printf("\n");
}
void chart(alp_t a[36], int l, int flag, int len){
	if(flag == 1){
		float fr=0;
		printf("\n");
		for(int i=0; i<l; i++){
			printf("   \u2502\n");
			printf(" %c \u2502", a[i].c);
			//bar((int)(fr*0.7));
			printf("\n   \u2502\n");
			printf("   \u2502\n");
		}
		printf("   \u2514");
		for(int j=0; j<76; ++j){
			printf("\u2500");
		}
		move_up(l*2);
		move_left(76);
		move_up(l*2);
		for(int i=0; i<l; i++){
			fr = (float)a[i].count*100/len;
			bar((int)(fr*0.7));
			move_down(1);
			printf("%.2f%c", fr, '%');
			move_down(3);
			move_left((int)(fr*0.7) + (int)(fr/10) + 5);
		}
		printf("\n");
	}else{
		float fr=0;
		printf("\n");
		for(int i=0; i<l; i++){
			printf("   \u2502\n");
			printf(" %c \u2502", a[i].c);
			//bar((int)(fr*0.7));
			printf("\n   \u2502\n");
			printf("   \u2502\n");
		}
		printf("   \u2514");
		for(int j=0; j<76; ++j){
			printf("\u2500");
		}
		move_up(l*4);
		move_left(76);
		float fr1 = (float)a[0].count*100/len;
		bar(80 - (((fr/10)>=1) + 10));
		move_down(1);
		printf("%.2f%c", fr1, '%');
		move_down(3);
		move_left(80 - (((fr/10)>=1) + 4));
		for(int i=1; i<l; i++){
			fr = (float)a[i].count*100/len;
			float fr2 = fr*70/fr1; 
			bar((int)(fr2));
			move_down(1);
			printf("%.2f%c", fr, '%');
			move_down(3);
			move_left((int)(fr2) + ((fr/10)>=1) + 5);
		}
		printf("\n");
	}
	
}
int max_len(int L, word_t a[L]){
	int M =0, length = 0;
	for(int i=0; i<L; i++){
		length = strlen(a[i].c);
		if(M<length){
			M = length;
		}
	}
	return M;
}
void chart_2(int l,word_t a[l],int flag, int len){
	int ma = max_len(l, a);
	if(a[0].count==len){
		printf("\n");
		float fr1 = (float)a[0].count*100/len;
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502");
			tape(80 - (((fr1/10)>=1)  + 9 + ma));
			printf("\n");
		printf(" %s", a[0].c);
		int r = strlen(a[0].c);
		for(int g=0; g<ma - r +1; g++){
			printf(" ");
		}
		printf("\u2502");
		tape(80 - (((fr1/10)>=1)  + 9 + ma));
		printf("%.2f%c\n", fr1, '%');
		fr1 = (float)a[0].count*100/len;
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502");
			tape(80 - (((fr1/10)>=1)  + 9 + ma));
			printf("\n");
		for(int g=0; g<ma + 2; g++){
			printf(" ");
		}
		printf("\u2502\n");
		for(int g=0; g<ma + 2; g++){
			printf(" ");
		}
		printf("\u2514");
		for(int j=0; j<80 - ma -3; ++j){
			printf("\u2500");
		}
		printf("\n");	
		return;
	}
	if(flag == 1){
		float fr1 = (float)a[0].count*100/len;
		float dii = (80 - ma - ((fr1/10)>=1) - 8)/100.0;
		float fr=0;
		printf("\n");
		for(int i=0; i<4*l; i++){
			fr = (float)a[i/4].count*100/len;
			if(i%4 == 0 || i%4==2){
				for(int g=0; g<ma + 2; g++){
					printf(" ");
				}
				printf("\u2502");
				tape((int)(fr*dii));
				printf("\n");
			}else{
				if(i%4==1){
					printf(" %s", a[i/4].c);
					int r = strlen(a[i/4].c);
					for(int g=0; g<ma - r +1; g++){
						printf(" ");
					}
					printf("\u2502");
					tape((int)(fr*dii));
					printf("%.2f%c\n", fr, '%');	
				}else{
					if(i%4==3){
						for(int g=0; g<ma + 2; g++){
							printf(" ");
						}
						printf("\u2502\n");
					}
				}
			}
		}
	}else{
		printf("\n");
		float fr1 = (float)a[0].count*100/len;
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502");
			tape(80 - (((fr1/10)>=1)  + 8 + ma));
			printf("\n");
		printf(" %s", a[0].c);
		int r = strlen(a[0].c);
		for(int g=0; g<ma - r +1; g++){
			printf(" ");
		}
		printf("\u2502");
		tape(80 - (((fr1/10)>=1)  + 8 + ma));
		printf("%.2f%c\n", fr1, '%');
		fr1 = (float)a[0].count*100/len;
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502");
			tape(80 - (((fr1/10)>=1)  + 8 + ma));
			printf("\n");
		for(int g=0; g<ma + 2; g++){
			printf(" ");
		}
		printf("\u2502\n");
		fr1 = (float)a[0].count*100/len;
		float dii = (80 - ma - ((fr1/10)>=1) - 8)/100.0;
		float fr=0;
		for(int i=4; i<4*l; i++){
			fr = (float)a[i/4].count*100/len;
			float fr2 = fr*(80 - (((fr1/10)>=1)  + 8 + ma))/fr1; 
			if(i%4 == 0 || i%4==2){
				for(int g=0; g<ma + 2; g++){
					printf(" ");
				}
				printf("\u2502");
				tape((int)fr2);
				printf("\n");
			}else{
				if(i%4==1){
					printf(" %s", a[i/4].c);
					int r = strlen(a[i/4].c);
					for(int g=0; g<ma - r +1; g++){
						printf(" ");
					}
					printf("\u2502");
					tape((int)fr2);
					printf("%.2f%c\n", fr, '%');	
				}else{
					if(i%4==3){
						for(int g=0; g<ma + 2; g++){
							printf(" ");
						}
						printf("\u2502\n");
					}
				}
			}
		}
	}
	for(int g=0; g<ma + 2; g++){
		printf(" ");
	}
	printf("\u2514");
	float fr1 = (float)a[0].count*100/len;
	float dii = (80 - ma - ((fr1/10)>=1) - 8)/100.0;
	for(int j=0; j<80 - ma -3; ++j){
		printf("\u2500");
	}
	printf("\n");	
}
void chart_w(int l,word_t a[l],int flag, int len){
	int ma = max_len(l, a);
	if(flag == 1){
		float fr=0;
		printf("\n");
		for(int i=0; i<l; i++){
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
			printf(" %s", a[i].c);
			int r = strlen(a[i].c);
			for(int g=0; g<ma - r + 1; g++){
				printf(" ");
			}
			printf("\u2502\n");
			//bar((int)(fr*0.7));
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
		}
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
		printf("\u2514");
		float fr1 = (float)a[0].count*100/len;
		float dii = (80 - ma - ((fr1/10)>=1) - 8)/100.0;
		for(int j=0; j<80 - ma -3; ++j){
			printf("\u2500");
		}
		move_up(l*4);
		move_left(80 - ma -3);
		for(int i=0; i<l; i++){
			fr = (float)a[i].count*100/len;
			bar((int)(fr*dii));
			move_down(1);
			printf("%.2f%c", fr, '%');
			move_down(3);
			move_left((int)(fr*dii) + ((fr/10)>=1) + 5);
		}
		printf("\n");
	}else{
		float fr=0;
		printf("\n");
		for(int i=0; i<l; i++){
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
			printf(" %s", a[i].c);
			int r = strlen(a[i].c);
			for(int g=0; g<ma - r + 1; g++){
				printf(" ");
			}
			printf("\u2502\n");
			//bar((int)(fr*0.7));
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
			for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
			printf("\u2502\n");
		}
		float fr1 = (float)a[0].count*100/len;
		for(int g=0; g<ma + 2; g++){
				printf(" ");
			}
		printf("\u2514");
		for(int j=0; j<80 - (ma + ((fr1/10)>=1) + 3); ++j){
			printf("\u2500");
		}
		move_up(l*4);
		move_left(80 - (ma + ((fr1/10)>=1) + 3));
		bar(80 - (((fr1/10)>=1)  + 8 + ma));
		move_down(1);
		printf("%.2f%c", fr1, '%');
		move_down(3);
		move_left(80 - (3 + ma));
		for(int i=1; i<l; i++){
			fr = (float)a[i].count*100/len;
			float fr2 = fr*(80 - (((fr1/10)>=1)  + 8 + ma))/fr1; 
			bar((int)(fr2));
			move_down(1);
			printf("%.2f%c", fr, '%');
			move_down(3);
			move_left((int)(fr2) + (int)(fr/10) + 5);
		}
		printf("\n");
	}
}