/*
 * A237884  a(n) = (n!*m)/(m!*(m+1)!) where m = floor(n/2).
 * https://oeis.org/A237884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n mod 2) * binomial(2*floor(n/2), floor(n/2)-1)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    idx_t m = n / 2;                       /* floor(n/2) */
    seq_t res = binomial(2 * m, m - 1);     /* C(2m, m-1) */
    if (n & 1)                              /* odd n: multiply by n */
        res = res * (seq_t)n;
    return res;
}
