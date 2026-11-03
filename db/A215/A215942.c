/*
 * A215942  a(n) = sigma(6*n) - 12*n.
 * https://oeis.org/A215942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(6*n) - 12*n
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 937 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of 6*n, then subtracts 12*n.   */
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
    /* 6*n fits in idx_t for the intended range of n. */
    idx_t m = n * (idx_t)6;

    /* Sum of divisors of m. */
    seq_t sigma = sigma_of(m);

    /* Subtract 12*n.  The value is guaranteed to be non‑negative. */
    seq_t sub = (seq_t)n * (seq_t)12;

    return sigma - sub;
}
