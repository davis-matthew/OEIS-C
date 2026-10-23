/*
 * A035609  Coordination sequence for lattice D*_20 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{20} 2^k·C(20,k)·C(n−1,k−1) + 2^20·C(n+9,19) with
 *            a(0) = 1. The standard binomial helper is used
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* first part: Σ_{k=1}^{20} 2^k * C(20,k) * C(n-1,k-1) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= 20U; ++k) {
        /* 2^k as seq_t */
        seq_t pow2 = (seq_t)1 << k;

        /* C(20,k) */
        seq_t c20k = binomial(20U, k);

        /* C(n-1,k-1) */
        seq_t cnk = binomial(n - 1U, k - 1U);

        /* term = pow2 * c20k * cnk */
        seq_t term = pow2 * c20k;
        term = term * cnk;

        sum = sum + term;
    }

    /* second part: 2^20 * C(n+9,19)  (since (20+2n)/2-1 = n+9) */
    seq_t pow2_20 = (seq_t)1 << 20U;
    seq_t cnk2 = binomial(n + 9U, 19U);

    sum = sum + (pow2_20 * cnk2);

    return sum;
}
