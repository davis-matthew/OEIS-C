/*
 * A239053  Sum of divisors of 4*n-1.
 * https://oeis.org/A239053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000203(4*n-1) = sum of divisors of (4*n-1)
 * Range:     u8 n <= 42, u16 n <= 8043, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9434 (2026-11-01)
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
    /* The sequence is defined only for n >= 1, but we guard against n==0 */
    if (n == 0) return 0;
    idx_t m = n * 4u - 1u;   /* 4*n-1 fits in idx_t for all practical n */
    return sigma_of(m);
}
