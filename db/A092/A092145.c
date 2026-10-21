/*
 * A092145  Numerator of I(n) = 2*(Integral_{x=0..1/2} (1+x^2)^n dx).
 * https://oeis.org/A092145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)/(4^k*(2*k+1)) * d, where d =
 *            2^(n-1)*(2*n+2)!/(n+1)! is A034910(n+1). The result is the
 *            numerator of the unreduced fraction I(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* compute denominator d = A034910(n+1) = 2^(n-1) * product_{i=n+2}^{2n+2} i */
    seq_t d = pow2(n - 1);        /* 2^(n-1) */
    for (idx_t i = n + 2; i <= 2 * n + 2; ++i) {
        d *= (seq_t)i;
    }

    seq_t sum = 0;
    seq_t binom = 1;          /* C(n,0) */
    seq_t pow4  = 1;          /* 4^0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t subden = pow4 * (seq_t)(2 * k + 1);          /* 4^k * (2k+1) */
        seq_t mult   = d / subden;                        /* d / subden  (exact) */
        sum += binom * mult;                              /* accumulate */

        if (k < n) {
            /* next binomial coefficient: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            pow4 *= 4;         /* raise 4 to the next power */
        }
    }

    return sum;
}
