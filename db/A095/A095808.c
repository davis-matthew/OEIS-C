/*
 * A095808  Number of ways to write n in the form m + (m+1) + ... + (m+k-1) +
 *          (m+k) + (m+k-1) + ... + (m+1) + m with integers m>= 1, k>=1. Or,
 *          number of divisors d of 4n-1 with 0 < (d-1)^2 < 4n.
 * https://oeis.org/A095808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(4*n-1) - 2) / 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   divisor_count_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(m) is the number of positive divisors of m. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_uint64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe return */

    uint64_t x = (uint64_t)4 * n - 1;    /* 4n-1 is odd and fits in 64 bits for n ≤ 10⁴  */
    uint64_t d  = divisor_count_uint64(x);
    d = (d - 2) / 2;                     /* integer division: result fits in seq_t   */
    return (seq_t)d;
}
