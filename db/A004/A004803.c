/*
 * A004803  Numbers that are the sum of 3 nonzero 10th powers.
 * https://oeis.org/A004803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   compare_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compare_sums.h"

/* a(n) = n-th smallest sum a^10 + b^10 + c^10 where 1 <= a <= b <= c */

static seq_t pow10(seq_t x) {
    seq_t result = 1;
    for (int i = 0; i < 10; i++) {
        result *= x;
    }
    return result;
}

seq_t a(idx_t n) {
    static seq_t *cached = NULL;
    static idx_t cached_len = 0;
    
    if (cached == NULL) {
        idx_t limit = 60;
        idx_t max_count = (limit + 2) * (limit + 1) * limit / 6;
        seq_t *sums = (seq_t *)malloc(max_count * sizeof(seq_t));
        idx_t count = 0;
        
        for (idx_t a = 1; a <= limit; a++) {
            seq_t a10 = pow10((seq_t)a);
            for (idx_t b = a; b <= limit; b++) {
                seq_t b10 = pow10((seq_t)b);
                for (idx_t c = b; c <= limit; c++) {
                    seq_t c10 = pow10((seq_t)c);
                    sums[count++] = a10 + b10 + c10;
                }
            }
        }
        
        qsort(sums, count, sizeof(seq_t), compare_sums);
        cached = sums;
        cached_len = count;
    }
    
    if (n > cached_len) {
        return 0;
    }
    
    return cached[n - 1];
}
