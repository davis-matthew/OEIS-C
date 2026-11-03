/*
 * A144613  a(n) = sigma(3*n) = A000203(3*n).
 * https://oeis.org/A144613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(3*n) = A000203(3*n)
 * Range:     u8 n <= 35, u16 n <= 5879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9567 (2026-11-01)
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
    /* compute 3*n, wrapping is acceptable because n is normally small
       relative to the 64‑bit limit; sigma_of will produce the exact
       result in seq_t */
    idx_t m = n * 3;
    return sigma_of(m);
}
