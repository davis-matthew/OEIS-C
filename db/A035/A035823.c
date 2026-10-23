/*
 * A035823  Coordination sequence for lattice D*_76 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{76} 2^k · binom(76,k) · binom(n−1,k−1) + 2^76 ·
 *            binom((76+2n)/2−1, 75)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;  /* a(0) = 1 */

    const idx_t N = 76;
    seq_t res = 0;

    /* first summation part */
    idx_t maxk = (n < N) ? n : N;   /* iterate only while k ≤ n and k ≤ N */
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t term = pow2(k) * binomial(N, k);        /* 2^k * binom(76,k) */
        term *= binomial(n - 1, k - 1);               /* * binom(n-1,k-1) */
        res += term;
    }

    /* second term */
    seq_t term2 = pow2(N) * binomial(n + N / 2 - 1, N - 1);
    res += term2;

    return res;
}
