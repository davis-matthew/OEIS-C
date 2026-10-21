/*
 * A067147  Triangle of coefficients for expressing x^n in terms of Hermite
 *          polynomials.
 * https://oeis.org/A067147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((-1+sqrt(1+8n))/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 0 if r-k is odd, T(r,k) = r! / (k! *
 *            ((r-k)/2)!) otherwise
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 153, u64 n <= 407,
 *            u128 n <= 1179, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* k is the column index within row r. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Determine parity of r-k. */
    idx_t diff = r - k;
    if ((diff & 1u) != 0)          /* odd difference -> entry is zero */
        return 0;

    /* Compute the factorial quotient.
       r!/(k!*((r-k)/2)!) = binom(r,k) * (diff)! / ((diff/2)!). */
    idx_t m = diff >> 1;           /* (r-k)/2 */
    seq_t result = binom(r, k);    /* r!/(k!*(r-k)!) */

    /* Multiply by (diff)! / m! : product of integers (m+1)..diff. */
    seq_t factor = 1;
    for (idx_t i = m + 1; i <= diff; ++i)
        factor *= (seq_t)i;

    return result * factor;
}
