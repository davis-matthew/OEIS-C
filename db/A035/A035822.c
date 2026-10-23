/*
 * A035822  Coordination sequence for lattice D*_74 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{k=1..74} 2^k * binomial(74, k) * binomial(n-1, k-1)
 *            * + 2^74 * binomial((74 + 2*n)/2 - 1, 73) * with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    const idx_t N = 74;               /* fixed dimension */

    if (n == 0)
        return (seq_t)1;              /* a(0) = 1 */

    seq_t sum = 0;

    /* first summation part */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t pk   = pow2(k);                         /* 2^k  */
        seq_t c1   = binomial(N, k);                  /* C(74,k) */
        seq_t c2   = binomial(n - 1, k - 1);          /* C(n-1,k-1) */
        sum += pk * c1 * c2;
    }

    /* second summation part */
    seq_t pow2_n = pow2(N);                                 /* 2^74 */
    idx_t t = (N + 2 * n) / 2 - 1;                          /* (74+2n)/2 - 1 */
    seq_t term2 = pow2_n * binomial(t, N - 1);              /* + 2^74 * C(t,73) */
    sum += term2;

    return sum;
}
