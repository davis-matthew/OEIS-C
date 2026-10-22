/*
 * A003384  Numbers that are the sum of 6 nonzero 8th powers.
 * https://oeis.org/A003384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   ascending enumeration of sums of 6 nonzero 8th powers
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   pow8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow8.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (cache == NULL) {
        seq_t *temp = (seq_t *)malloc(1000000 * sizeof(seq_t));
        idx_t count = 0;
        
        for (int a = 1; a <= 25; a++) {
            seq_t a8 = pow8((seq_t)a);
            for (int b = a; b <= 25; b++) {
                seq_t b8 = pow8((seq_t)b);
                for (int c = b; c <= 25; c++) {
                    seq_t c8 = pow8((seq_t)c);
                    for (int d = c; d <= 25; d++) {
                        seq_t d8 = pow8((seq_t)d);
                        for (int e = d; e <= 25; e++) {
                            seq_t e8 = pow8((seq_t)e);
                            for (int f = e; f <= 25; f++) {
                                seq_t f8 = pow8((seq_t)f);
                                temp[count++] = a8 + b8 + c8 + d8 + e8 + f8;
                            }
                        }
                    }
                }
            }
        }
        
        qsort(temp, count, sizeof(seq_t), cmp_seq);
        
        idx_t unique = 0;
        for (idx_t i = 0; i < count; i++) {
            if (unique == 0 || temp[i] != temp[unique - 1]) {
                temp[unique++] = temp[i];
            }
        }
        
        cache = temp;
        cache_len = unique;
    }
    
    if (n > 0 && n <= cache_len) {
        return cache[n - 1];
    }
    
    return 0;
}
