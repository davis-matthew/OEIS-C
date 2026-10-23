/*
 * A120787  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/20.
 * https://oeis.org/A120787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of the rational number r(n) = Σ_{k=0}^n
 *            Catalan(k) / 20^k. The denominator is 20^n / gcd( N(n), 20^n ),
 *            where N(n) = Σ_{k=0}^n Catalan(k)·20^{n-k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   catalan_vp, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp)   */
#include "../utils/power5.h"   /* seq_t power5(idx_t exp) */
#include "../utils/catalan_vp.h"

seq_t a(idx_t n)
{
    /* Precompute the minimal 2- and 5-exponents appearing in N(n). */
    idx_t min_v2 = 2ULL * n + 4;   /* large enough initial value   */
    idx_t min_v5 = n + 4;          /* large enough initial value   */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t v2 = catalan_vp(i, 2);
        idx_t v5 = catalan_vp(i, 5);

        idx_t cand2 = v2 + 2ULL * (n - i);
        if (cand2 < min_v2) min_v2 = cand2;

        idx_t cand5 = v5 + (n - i);
        if (cand5 < min_v5) min_v5 = cand5;
    }

    /* Exponents of 2 and 5 that survive in the denominator. */
    idx_t exp2 = 2ULL * n - min_v2;
    idx_t exp5 = n - min_v5;

    /* Compute 2^exp2 and 5^exp5 and multiply. */
    seq_t denom = pow2(exp2) * power5(exp5);
    return denom;
}
