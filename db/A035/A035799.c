/*
 * A035799  Coordination sequence for lattice D*_28 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{28} 2^k * binomial(28, k) * binomial(n-1, k-1) +
 *            2^28 * binomial(n+13, 27) (with a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use the binomial and pow2 utilities */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return (seq_t)1;

    const idx_t N = 28;       /* fixed dimension */
    seq_t sum   = (seq_t)0;
    idx_t maxk = (n < N) ? n : N;   /* k runs 1..min(N,n) */

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t term = binomial(N, k);          /* binomial(28, k)   */
        term *= pow2(k);                      /* * 2^k             */
        term *= binomial(n - 1, k - 1);       /* * binomial(n-1,k-1) */
        sum += term;
    }

    seq_t term2 = pow2(N) * binomial(n + 13, N - 1);  /* 2^28 * C(n+13,27) */
    return sum + term2;
}
