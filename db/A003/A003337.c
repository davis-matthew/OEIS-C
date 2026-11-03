/*
 * A003337  Numbers which are the sum of 3 nonzero 4th powers.
 * https://oeis.org/A003337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers which are the sum of 3 nonzero 4th powers
 * Range:     u8 n <= 7, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9749 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (cache == NULL) {
        seq_t limit = 50000000;
        
        idx_t capacity = 100000;
        seq_t *temp = mem_new(seq_t, capacity);
        idx_t count = 0;
        
        for (seq_t a = 1; a * a * a * a <= limit; a++) {
            seq_t a4 = a * a * a * a;
            for (seq_t b = a; a4 + b * b * b * b <= limit; b++) {
                seq_t ab4 = a4 + b * b * b * b;
                for (seq_t c = b; ab4 + c * c * c * c <= limit; c++) {
                    seq_t sum = ab4 + c * c * c * c;
                    if (count >= capacity) {
                        capacity *= 2;
                        seq_t *new_temp = mem_new(seq_t, capacity);
                        for (idx_t i = 0; i < count; i++) {
                            new_temp[i] = temp[i];
                        }
                        mem_free(temp);
                        temp = new_temp;
                    }
                    temp[count++] = sum;
                }
            }
        }
        
        mem_qsort(temp, count, sizeof(seq_t), cmp_seq);
        
        idx_t unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || temp[i] != temp[i-1]) {
                unique_count++;
            }
        }
        
        cache = mem_new(seq_t, unique_count);
        unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || temp[i] != temp[i-1]) {
                cache[unique_count++] = temp[i];
            }
        }
        cache_size = unique_count;
        mem_free(temp);
    }
    
    return (n <= cache_size) ? cache[n - 1] : 0;
}
