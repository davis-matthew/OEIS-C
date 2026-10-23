/*
 * A117266  Row sums of triangle A117265.
 * https://oeis.org/A117266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*(n-k))! /
 *            ((n-k)!*(n-k)!)*2^((n-k)*(n+k-3)/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m;
    seq_t sum = 0;
    seq_t binom = 1;           /* binom = C(0,0) for m = 0 */

    for (m = 0; m <= n; ++m)
    {
        /* exponent t = m*(2*n - m - 3)/2  (may be negative) */
        int64_t temp = (int64_t)m * (int64_t)(2 * (int64_t)n - (int64_t)m - 3);
        int64_t t = temp / 2;

        seq_t term;
        if (t >= 0)
        {
            term = binom << (idx_t)t;
        }
        else
        {
            term = binom >> (idx_t)(-t);
        }

        sum += term;

        /* compute C(2(m+1), m+1) for next iteration if needed */
        if (m < n)
        {
            seq_t numer = binom * (seq_t)(4 * m + 2);
            binom = numer / (seq_t)(m + 1);
        }
    }

    return sum;
}
