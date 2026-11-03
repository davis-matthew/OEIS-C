/*
 * A003547  Divisors of 2^42 - 1.
 * https://oeis.org/A003547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 2^42 - 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 98, u64 n <= 144, u128 n <= 144,
 *            big any n
 * Validated: full, pass, 144 terms through n = 144 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int compare_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

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
        mem_qsort(divisors, divisor_count, sizeof(seq_t), compare_seq);
    }
    
    return divisors[(int)n - 1];
}
