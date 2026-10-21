/*
 * A094114  a(n) = -Sum_{i=1..n-1} (-1)^i*2^valuation(i,3).
 * https://oeis.org/A094114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -∑_{i=1}^{n-1} (-1)^i * 2^valuation(i,3)
 * Range:     u8 n <= 6561, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 */

    seq_t sum = 0;                /* S = ∑_{i=1}^{n-1} (-1)^i * 2^{v3(i)} */
    for (idx_t i = 1; i < n; ++i)
    {
        /* compute v3(i) */
        idx_t t = i;
        idx_t e = 0;
        while ((t % 3) == 0)
        {
            ++e;
            t /= 3;
        }

        /* (-1)^i: +1 if i even, -1 if odd */
        seq_t sign = (i & 1) ? -1 : 1;

        /* 2^e using shift */
        seq_t two_pow = ((seq_t)1) << e;

        sum += sign * two_pow;
    }

    return -sum;
}
