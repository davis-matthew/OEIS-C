/*
 * A035833  Coordination sequence for lattice D*_96 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n==0?1: Σ_{k=1}^{min(96,n)} 2^k * C(96,k) * C(n-1,k-1) +
 *            2^96 * C(n+47,95) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* Constants */
    const idx_t N = 96;
    seq_t sum = (seq_t)0;

    /* Upper limit for k */
    idx_t upper_k = (n < N) ? n : N;

    /* Sum over k = 1 .. upper_k */
    for (idx_t k = 1; k <= upper_k; ++k) {
        /* term = 2^k * C(96,k) * C(n-1,k-1) */
        seq_t term = pow2(k) * binomial(N, k) * binomial(n - 1, k - 1);
        sum += term;
    }

    /* Add the second part: 2^96 * C(n+47, 95) */
    seq_t term2 = pow2(N) * binomial(n + (idx_t)47, N - 1);
    sum += term2;

    return sum;
}
