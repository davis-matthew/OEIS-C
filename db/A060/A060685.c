/*
 * A060685  Largest difference between consecutive divisors (ordered by size)
 *          of 2n+1.
 * https://oeis.org/A060685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1) - (2n+1)/p where p is the smallest prime divisor
 *            of (2n+1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2n+1 */
    seq_t m = (seq_t)2 * n + 1;

    /* smallest prime divisor p of m (m is odd, so skip 2) */
    seq_t p = 0;
    for (idx_t d = 3; (seq_t)d * (seq_t)d <= m; d += 2)
    {
        if (m % d == 0)
        {
            p = d;
            break;
        }
    }
    if (!p) /* m is prime */
        p = m;

    /* largest consecutive divisor difference */
    return m - m / p;
}
