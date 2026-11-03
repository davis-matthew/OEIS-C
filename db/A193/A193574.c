/*
 * A193574  Smallest divisor of sigma(n) that does not divide n.
 * https://oeis.org/A193574
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of sigma(n) that does not divide n
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9626 (2026-11-01)
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

seq_t a(idx_t n) {
    /* n >= 2 as per OEIS offset. Find sigma(n). */
    seq_t sigma = sigma_of(n);
    /* Convert to idx_t for looping; sigma fits into idx_t for n <= 10000 */
    idx_t sigma_val = (idx_t)sigma;

    for (idx_t d = 2; d <= sigma_val; ++d) {
        if (sigma_val % d == 0) {
            if (n % d != 0) {
                return (seq_t)d;
            }
        }
    }
    /* Fallback: sigma itself (will never be divisible by n for n >= 2) */
    return sigma;
}
