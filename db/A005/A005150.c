/*
 * A005150  Look and Say sequence: describe the previous term! (method A -
 *          initial term is 1).
 * https://oeis.org/A005150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Look-and-say sequence, a(n+1) = A045918(a(n)), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char **memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (memo_len == 0) {
        memo = (char **)malloc(11 * sizeof(char *));
        memo_len = 10;
        for (idx_t i = 0; i <= 10; i++) {
            memo[i] = NULL;
        }
    }
    
    while (n > memo_len) {
        idx_t new_len = memo_len * 2;
        memo = (char **)realloc(memo, (new_len + 1) * sizeof(char *));
        for (idx_t i = memo_len + 1; i <= new_len; i++) {
            memo[i] = NULL;
        }
        memo_len = new_len;
    }
    
    for (idx_t i = 1; i <= n; i++) {
        if (memo[i] == NULL) {
            if (i == 1) {
                memo[1] = (char *)malloc(2);
                strcpy(memo[1], "1");
            } else {
                char *prev = memo[i - 1];
                size_t prev_len = strlen(prev);
                char *next = (char *)malloc(prev_len * 3 + 10);
                int pos = 0;
                
                int j = 0;
                while (prev[j] != '\0') {
                    char digit = prev[j];
                    int count = 0;
                    while (prev[j] == digit) {
                        count++;
                        j++;
                    }
                    pos += sprintf(next + pos, "%d%c", count, digit);
                }
                next[pos] = '\0';
                memo[i] = next;
            }
        }
    }
    
    char *digits = memo[n];
    seq_t result = 0;
    for (size_t i = 0; digits[i] != '\0'; i++) {
        result = result * 10 + (digits[i] - '0');
    }
    return result;
}
