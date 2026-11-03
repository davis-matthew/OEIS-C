/*
 * A063445  Moebius transform of f(x) = EulerPhi(x^2) function (A002618).
 * https://oeis.org/A063445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} phi(d^2) * mu(n/d) (OEIS A063445)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8923 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/totient_seq.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

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
        seq_t phi_ds = totient_seq(d_sq);
        int mu1 = mu_of(d2);                /* mu(n/d) where d2 = n/d */
        if (mu1 != 0) sum += mu1 * (int64_t)phi_ds;

        /* if the paired divisor is distinct, add its contribution */
        if (d2 != d) {
            seq_t d2_sq   = (seq_t)d2 * (seq_t)d2;
            seq_t phi_d2s = totient_seq(d2_sq);
            int mu2 = mu_of(d);             /* mu(n/d2) where d = n/d2 */
            if (mu2 != 0) sum += mu2 * (int64_t)phi_d2s;
        }
    }

    return (seq_t)sum;
}
