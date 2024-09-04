#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FRAMES 100
#define MAX_REFERENCES 1000

int find_optimal(int frames[], int ref_str[], int n, int index, int frame_size) {
    int farthest = index;
    int res = -1;

    for (int i = 0; i < frame_size; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == ref_str[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }

        if (j == n)
            return i;
    }

    return (res == -1) ? 0 : res;
}

bool is_in_frame(int frames[], int frame_size, int page) {
    for (int i = 0; i < frame_size; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

void optimal_page_replacement(int ref_str[], int n, int frame_size) {
    int frames[MAX_FRAMES];
    int page_faults = 0;

    for (int i = 0; i < frame_size; i++) frames[i] = -1;

    printf("Replacement Policy = OPTIMAL\n-------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%02d ", ref_str[i]);
        if (!is_in_frame(frames, frame_size, ref_str[i])) {
            int index = find_optimal(frames, ref_str, n, i + 1, frame_size);
            frames[index] = ref_str[i];
            page_faults++;
            printf("F ");
        }
        for (int j = 0; j < frame_size; j++) {
            if (frames[j] != -1) printf("%02d ", frames[j]);
        }
        printf("\n");
    }

    printf("-------------------------------------\nNumber of page faults = %d\n", page_faults);
}

void fifo_page_replacement(int ref_str[], int n, int frame_size) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int next_replace = 0;

    for (int i = 0; i < frame_size; i++) frames[i] = -1;

    printf("Replacement Policy = FIFO\n-------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%02d ", ref_str[i]);
        if (!is_in_frame(frames, frame_size, ref_str[i])) {
            frames[next_replace] = ref_str[i];
            next_replace = (next_replace + 1) % frame_size;
            page_faults++;
            printf("F ");
        }
        for (int j = 0; j < frame_size; j++) {
            if (frames[j] != -1) printf("%02d ", frames[j]);
        }
        printf("\n");
    }

    printf("-------------------------------------\nNumber of page faults = %d\n", page_faults);
}

void lru_page_replacement(int ref_str[], int n, int frame_size) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int lru[MAX_FRAMES];

    for (int i = 0; i < frame_size; i++) {
        frames[i] = -1;
        lru[i] = -1;
    }

    printf("Replacement Policy = LRU\n-------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%02d ", ref_str[i]);
        if (!is_in_frame(frames, frame_size, ref_str[i])) {
            int min_index = 0;
            for (int j = 1; j < frame_size; j++) {
                if (lru[j] < lru[min_index]) {
                    min_index = j;
                }
            }
            frames[min_index] = ref_str[i];
            lru[min_index] = i;
            page_faults++;
            printf("F ");
        } else {
            for (int j = 0; j < frame_size; j++) {
                if (frames[j] == ref_str[i]) {
                    lru[j] = i;
                    break;
                }
            }
        }
        for (int j = 0; j < frame_size; j++) {
            if (frames[j] != -1) printf("%02d ", frames[j]);
        }
        printf("\n");
    }

    printf("-------------------------------------\nNumber of page faults = %d\n", page_faults);
}

void clock_page_replacement(int ref_str[], int n, int frame_size) {
    int frames[MAX_FRAMES];
    int use_bit[MAX_FRAMES];
    int page_faults = 0;
    int pointer = 0;

    for (int i = 0; i < frame_size; i++) {
        frames[i] = -1;
        use_bit[i] = 0;
    }

    printf("Replacement Policy = CLOCK\n-------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%02d ", ref_str[i]);
        if (!is_in_frame(frames, frame_size, ref_str[i])) {
            while (use_bit[pointer] == 1) {
                use_bit[pointer] = 0;
                pointer = (pointer + 1) % frame_size;
            }
            frames[pointer] = ref_str[i];
            use_bit[pointer] = 1;
            pointer = (pointer + 1) % frame_size;
            page_faults++;
            printf("F ");
        } else {
            for (int j = 0; j < frame_size; j++) {
                if (frames[j] == ref_str[i]) {
                    use_bit[j] = 1;
                    break;
                }
            }
        }
        for (int j = 0; j < frame_size; j++) {
            if (frames[j] != -1) printf("%02d ", frames[j]);
        }
        printf("\n");
    }

    printf("-------------------------------------\nNumber of page faults = %d\n", page_faults);
}

int main() {
    int frame_size;
    char algorithm[10];
    int ref_str[MAX_REFERENCES];
    int n = 0;

    // Read input
    scanf("%d", &frame_size);
    scanf("%s", algorithm);
    while (scanf("%d", &ref_str[n]) && ref_str[n] != -1) {
        n++;
    }

    // Choose algorithm
    if (strcmp(algorithm, "OPTIMAL") == 0) {
        optimal_page_replacement(ref_str, n, frame_size);
    } else if (strcmp(algorithm, "FIFO") == 0) {
        fifo_page_replacement(ref_str, n, frame_size);
    } else if (strcmp(algorithm, "LRU") == 0) {
        lru_page_replacement(ref_str, n, frame_size);
    } else if (strcmp(algorithm, "CLOCK") == 0) {
        clock_page_replacement(ref_str, n, frame_size);
    } else {
        printf("Invalid algorithm!\n");
    }

    return 0;
}
