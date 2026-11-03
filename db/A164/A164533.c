/*
 * A164533  a(n) = sigma(sigma(n))*sigma(n).
 * https://oeis.org/A164533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(sigma(n))*sigma(n)
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9410 (2026-11-01)
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
    /* The sequence is defined only for n ≥ 1 (offset 1). */
    if (n == 0)
        return (seq_t)0;   /* behavior below offset is irrelevant */

    seq_t sigma_n   = sigma_of(n);              /* sigma(n)   */
    seq_t sigma_sig = sigma_of((idx_t)sigma_n); /* sigma(sigma(n)) */

    return sigma_n * sigma_sig;
}
