/*
 * A022249  Gaussian binomial coefficients [ n,9 ] for q = 8.
 * https://oeis.org/A022249
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,9] for q = 8 Offset: 9
 *            a(n) = Π_{i=1..9} (8^{n-i+1} - 1) / (8^i - 1) (exact integer)
 *            The values are obtained by the standard recurrence for Gaussian
 *            binomial coefficients: C_q(n,k) = C_q(n-1,k-1) + q^k *
 *            C_q(n-1,k) (q = 8, k = 9) with C_q(n,0) = C_q(n,n) =...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Values are defined for n >= 9; below the offset the behaviour is irrelevant. */
    if (n < 9)
        return 0;

    seq_t v[10] = {0};
    v[0] = 1;                                 /* C(n,0) = 1 for all n */

    for (idx_t m = 1; m <= n; ++m)
    {
        int lim = (m < 9 ? m : 9);             /* we only need values up to k = 9 */
        for (int k = lim; k >= 1; --k)
        {
            /* C_q(m,k) = C_q(m-1,k-1) + q^k * C_q(m-1,k)
               where q = 8 and q^k = 2^{3k} */
            v[k] = v[k - 1] + (v[k] << (3 * k));
        }
    }
    return v[9];                               /* C_q(n,9) */
}
