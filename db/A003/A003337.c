/*
 * A003337  Numbers which are the sum of 3 nonzero 4th powers.
 * https://oeis.org/A003337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers which are the sum of 3 nonzero 4th powers
 * Range:     u8 n <= 7, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (cache == NULL) {
        seq_t limit = 50000000;
        
        idx_t capacity = 100000;
        seq_t *temp = (seq_t *)malloc(capacity * sizeof(seq_t));
        idx_t count = 0;
        
        for (seq_t a = 1; a * a * a * a <= limit; a++) {
            seq_t a4 = a * a * a * a;
            for (seq_t b = a; a4 + b * b * b * b <= limit; b++) {
                seq_t ab4 = a4 + b * b * b * b;
                for (seq_t c = b; ab4 + c * c * c * c <= limit; c++) {
                    seq_t sum = ab4 + c * c * c * c;
                    if (count >= capacity) {
                        capacity *= 2;
                        seq_t *new_temp = (seq_t *)malloc(capacity * sizeof(seq_t));
                        for (idx_t i = 0; i < count; i++) {
                            new_temp[i] = temp[i];
                        }
                        free(temp);
                        temp = new_temp;
                    }
                    temp[count++] = sum;
                }
            }
        }
        
        qsort(temp, count, sizeof(seq_t), cmp_seq);
        
        idx_t unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || temp[i] != temp[i-1]) {
                unique_count++;
            }
        }
        
        cache = (seq_t *)malloc(unique_count * sizeof(seq_t));
        unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || temp[i] != temp[i-1]) {
                cache[unique_count++] = temp[i];
            }
        }
        cache_size = unique_count;
        free(temp);
    }
    
    return (n <= cache_size) ? cache[n - 1] : 0;
}
