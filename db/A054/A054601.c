/*
 * A054601  a(n) = Sum_{d|n, d odd} d*2^(n/d - 1), a(0)=0.
 * https://oeis.org/A054601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d odd} d * 2^(n/d - 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 2^exp for an arbitrary precision seq_t */
static seq_t pow2(uint64_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp)
    {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if (d & 1u)                     /* d is odd */
            {
                idx_t m = n / d;            /* exponent m-1 = n/d - 1 */
                sum += (seq_t)d * pow2(m - 1);
            }

            /* complementary divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && (d2 & 1u))
            {
                idx_t m = d;                /* because n/d2 = d */
                sum += (seq_t)d2 * pow2(m - 1);
            }
        }
    }
    return sum;
}
