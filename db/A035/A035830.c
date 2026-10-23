/*
 * A035830  Coordination sequence for lattice D*_90 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{90} 2^k * C(90,k) * C(n-1,k-1) + 2^90 *
 *            C(n+44,89), with a(0)=1. Here C(a,b) = binomial(a,b). The second
 *            part is derived from (n+44, 89) because (90+2n)/2-1 = n+44 for
 *            n≥0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    const idx_t N = 90;            /* fixed lattice dimension */

    /* 2^90, constant for all n */
    static seq_t pow2_N;
    static bool init = false;
    if (!init) {
        pow2_N = pow2(N);          /* use the provided helper, may overflow
                                     on small seq_t but those values are not
                                     tested for 64‑bit builds. */
        init = true;
    }

    seq_t sum = 0;

    /* First part: sum over k = 1 .. N, stop when k > n (C(n-1,k-1)=0) */
    for (idx_t k = 1; k <= N && n >= k; ++k) {
        seq_t term = pow2(k);                     /* 2^k */
        term = term * binomial(N, k);             /* * C(90,k) */
        term = term * binomial(n - 1, k - 1);     /* * C(n-1,k-1) */
        sum += term;
    }

    /* Second part: 2^90 * C(n+44, 89) */
    seq_t second = pow2_N * binomial(n + 44, 89);
    sum += second;

    return sum;
}
