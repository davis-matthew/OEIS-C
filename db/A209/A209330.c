/*
 * A209330  Triangle defined by T(n,k) = binomial(n^2, n*k), for n>=0, k=0..n,
 *          as read by rows.
 * https://oeis.org/A209330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(m^2, m*k) where m is the unique integer satisfying
 *            m*(m+1)/2 <= n < (m+1)*(m+2)/2 and k = n - m*(m+1)/2
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 22, u64 n <= 47, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t k = n - m * (m + 1) / 2;
    return binomial(m * m, m * k);
}
