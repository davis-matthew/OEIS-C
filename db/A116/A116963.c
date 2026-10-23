/*
 * A116963  Inverse Moebius transform of the shifted tetrahedral numbers.
 * https://oeis.org/A116963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} (d+1)(d+2)(d+3)/6 = (σ₃(n) + 6σ₂(n) + 11σ₁(n) +
 *            6σ₀(n))/6 (σ_k = sum of k‑th powers of divisors)
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 1796, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n ≥ 1 but keep safe */
    seq_t sigma0 = 0;   /* number of divisors (σ₀)          */
    seq_t sigma1 = 0;   /* sum of divisors (σ₁)            */
    seq_t sigma2 = 0;   /* sum of squares of divisors (σ₂) */
    seq_t sigma3 = 0;   /* sum of cubes of divisors (σ₃)  */

    /* Enumerate divisors by trial division up to √n. */
    idx_t d = 1;
    for (; d * d <= n; ++d) {
        if (n % d == 0) {
            /* short hand for the divisor and its counterpart */
            seq_t dd  = (seq_t)d;
            seq_t dd2 = (seq_t)(n / d);
            if (d == n / d) {           /* perfect square case */
                sigma0 += 1;
                sigma1 += dd;
                sigma2 += dd * dd;
                sigma3 += dd * dd * dd;
            } else { /* two distinct divisors */
                sigma0 += 2;
                sigma1 += dd + dd2;
                sigma2 += dd * dd + dd2 * dd2;
                sigma3 += dd * dd * dd + dd2 * dd2 * dd2;
            }
        }
    }

    /* Combine the sums according to the closed formula. */
    seq_t sum = sigma3 + 6 * sigma2 + 11 * sigma1 + 6 * sigma0;
    return sum / 6;
}
