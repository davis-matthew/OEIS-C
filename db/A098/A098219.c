/*
 * A098219  a(n) = floor(sigma(sigma(n))/n) = floor(A051027(n)/n).
 * https://oeis.org/A098219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sigma(sigma(n)) / n)
 * Range:     u8 n <= 53, u16 n <= 24999, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 121 terms through n = 9658 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    seq_t sigma1 = sigma_of((idx_t)n);
    seq_t sigma2 = sigma_of((idx_t)sigma1);
    return sigma2 / (seq_t)n;
}
