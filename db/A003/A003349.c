/*
 * A003349  Numbers that are the sum of 4 positive 5th powers.
 * https://oeis.org/A003349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is the sum of 4 positive 5th powers
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static int max_base = 0;

static int cmp_seq_t(const void *a, const void *b) {
    if (*(const seq_t *)a < *(const seq_t *)b) return -1;
    if (*(const seq_t *)a > *(const seq_t *)b) return 1;
    return 0;
}

static void generate_sums(int max_b) {
    if (max_b <= max_base) return;
    
    idx_t capacity = 100000;
    seq_t *temp = (seq_t *)malloc(capacity * sizeof(seq_t));
    idx_t count = 0;
    
    for (int a = 1; a <= max_b; a++) {
        seq_t a5 = (seq_t)a * a * a * a * a;
        for (int b = a; b <= max_b; b++) {
            seq_t b5 = (seq_t)b * b * b * b * b;
            for (int c = b; c <= max_b; c++) {
                seq_t c5 = (seq_t)c * c * c * c * c;
                for (int d = c; d <= max_b; d++) {
                    seq_t d5 = (seq_t)d * d * d * d * d;
                    seq_t sum = a5 + b5 + c5 + d5;
                    
                    if (count >= capacity) {
                        capacity *= 2;
                        temp = (seq_t *)realloc(temp, capacity * sizeof(seq_t));
                    }
                    temp[count++] = sum;
                }
            }
        }
    }
    
    qsort(temp, count, sizeof(seq_t), cmp_seq_t);
    
    idx_t unique_count = 0;
    for (idx_t i = 0; i < count; i++) {
        if (i == 0 || temp[i] != temp[i-1]) {
            temp[unique_count++] = temp[i];
        }
    }
    
    free(cache);
    cache = temp;
    cache_len = unique_count;
    max_base = max_b;
}

seq_t a(idx_t n) {
    int base = 35;
    while (base <= 200) {
        generate_sums(base);
        if (n <= cache_len) {
            return cache[n - 1];
        }
        base += 20;
    }
    return 0;
}
