/*
 * A035816  Coordination sequence for lattice D*_62 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{62} 2^k · C(62,k) · C(n‑1,k‑1) + 2^62 ·
 *            C((62+2n)/2 – 1, 61) (where C(a,b)=0 if b<0 or b>a). The special
 *            case a(0)=1 is handled separately
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    const idx_t N = 62;
    seq_t sum = 0;
    seq_t pow2N = pow2(N);              /* 2^62 once */

    /* First summation: k = 1 … 62 (k=0 gives a zero term) */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = pow2(k);                   /* 2^k */
        term *= binomial(N, k);                /* * C(62,k) */
        term *= binomial(n - 1, k - 1);        /* * C(n‑1,k‑1) */
        sum += term;
    }

    /* Second term */
    idx_t arg = (N + 2 * n) / 2 - 1;            /* ((62+2n)/2 – 1) */
    sum += pow2N * binomial(arg, N - 1);        /* 2^62 · C(arg,61) */

    return sum;
}
