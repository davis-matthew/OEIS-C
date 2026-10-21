/*
 * A227403  a(n) = Sum_{k=0..n} binomial(n^2, n*k) * binomial(n*k, k^2).
 * https://oeis.org/A227403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n^2, n*k) * binomial(n*k, k^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute n^2 once */
    idx_t N = n * n;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t nk = n * k;   /* n*k  */
        idx_t k2 = k * k;   /* k^2  */

        seq_t term1 = binomial(N, nk);
        seq_t term2 = binomial(nk, k2);

        sum += term1 * term2;
    }

    return sum;
}
