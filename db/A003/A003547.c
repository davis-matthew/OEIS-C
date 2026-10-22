/*
 * A003547  Divisors of 2^42 - 1.
 * https://oeis.org/A003547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 2^42 - 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 98, u64 n <= 144, u128 n <= 144,
 *            big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compare_seq.h"

static seq_t divisors[1000];
static int divisor_count = -1;

seq_t a(idx_t n) {
    if (divisor_count == -1) {
        uint64_t target = ((uint64_t)1 << 42) - 1;
        
        int cnt = 0;
        for (uint64_t i = 1; i <= target / i; i++) {
            if (target % i == 0) {
                divisors[cnt++] = (seq_t)i;
                uint64_t j = target / i;
                if (i != j) {
                    divisors[cnt++] = (seq_t)j;
                }
            }
        }
        
        divisor_count = cnt;
        qsort(divisors, divisor_count, sizeof(seq_t), compare_seq);
    }
    
    return divisors[(int)n - 1];
}
