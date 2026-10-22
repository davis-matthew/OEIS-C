/*
 * A003538  Divisors of 2^30 - 1.
 * https://oeis.org/A003538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^30 - 1
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compare_seq.h"

seq_t a(idx_t n) {
    static seq_t *divisors = NULL;
    static int computed = 0;
    
    if (!computed) {
        /* 2^30 - 1 = 3^2 * 7 * 11 * 31 * 151 * 331 */
        seq_t p3[] = {1, 3, 9};
        seq_t p7[] = {1, 7};
        seq_t p11[] = {1, 11};
        seq_t p31[] = {1, 31};
        seq_t p151[] = {1, 151};
        seq_t p331[] = {1, 331};
        
        divisors = (seq_t *)malloc(96 * sizeof(seq_t));
        int idx = 0;
        
        for (int e0 = 0; e0 < 3; e0++) {
            for (int e1 = 0; e1 < 2; e1++) {
                for (int e2 = 0; e2 < 2; e2++) {
                    for (int e3 = 0; e3 < 2; e3++) {
                        for (int e4 = 0; e4 < 2; e4++) {
                            for (int e5 = 0; e5 < 2; e5++) {
                                divisors[idx++] = p3[e0] * p7[e1] * p11[e2] * p31[e3] * p151[e4] * p331[e5];
                            }
                        }
                    }
                }
            }
        }
        
        qsort(divisors, 96, sizeof(seq_t), compare_seq);
        computed = 1;
    }
    
    if (n < 1 || n > 96) {
        return 0;
    }
    return divisors[n - 1];
}
