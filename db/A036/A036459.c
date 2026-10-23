/*
 * A036459  Number of iterations required to reach stationary value when
 *          repeatedly applying d, the number of divisors function (A000005).
 * https://oeis.org/A036459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations required to reach 2 via repeated
 *            applications of d(n), where d is the divisor-count function
 *            (A000005). a(1) = 0, a(2) = 0; for n > 2, a(n) = a(d(n)) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"   /* declares uint64_t divisor_count_u64(uint64_t n) */

/* Public function */
seq_t a(idx_t n)
{
    /* a(1) and a(2) are explicitly defined as 0 */
    if (n <= 2)
        return 0;

    seq_t steps = 0;
    uint64_t cur = (uint64_t)n;

    /* iterate until the value becomes 1 or 2, but for all n>2 it will hit 2 */
    while (cur > 2) {
        cur = divisor_count_u64(cur);
        steps++;
    }
    return steps;
}
