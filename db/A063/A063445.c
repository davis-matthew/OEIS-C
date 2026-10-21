/*
 * A063445  Moebius transform of f(x) = EulerPhi(x^2) function (A002618).
 * https://oeis.org/A063445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} phi(d^2) * mu(n/d) (OEIS A063445)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"   /* calc_phi(seq_t m) -> seq_t */
#include "../utils/mu_of.h"     /* mu_of(idx_t m) -> int */

seq_t a(idx_t n)
{
    /* a(1) = 1 is handled correctly by the generic formula. */
    if (n == 0) return 0;          /* safeguard – not part of the sequence */

    int64_t sum = 0;                /* signed accumulator, safe up to n≈10⁴ */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;          /* d is not a divisor */

        idx_t d2 = n / d;                   /* paired divisor */

        /* contribution from divisor d */
        seq_t d_sq   = (seq_t)d * (seq_t)d;
        seq_t phi_ds = calc_phi(d_sq);
        int mu1 = mu_of(d2);                /* mu(n/d) where d2 = n/d */
        if (mu1 != 0) sum += mu1 * (int64_t)phi_ds;

        /* if the paired divisor is distinct, add its contribution */
        if (d2 != d) {
            seq_t d2_sq   = (seq_t)d2 * (seq_t)d2;
            seq_t phi_d2s = calc_phi(d2_sq);
            int mu2 = mu_of(d);             /* mu(n/d2) where d = n/d2 */
            if (mu2 != 0) sum += mu2 * (int64_t)phi_d2s;
        }
    }

    return (seq_t)sum;
}
