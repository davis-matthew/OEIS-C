/*
 * A092411  a(n) = sigma(n,2) + sigma(n+1,2).
 * https://oeis.org/A092411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n,2) + sigma(n+1,2) where sigma(m,2) = Σ_{d|m} d^2.
 *            The implementation uses a simple O(√m) divisor enumeration
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Compute the sum of squares of divisors of m. */
static seq_t sigma2(idx_t m)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = (idx_t)1; d * d <= m; ++d) {
        if (m % d == (idx_t)0) {
            seq_t d_sq = (seq_t)d * (seq_t)d;
            sum += d_sq;
            idx_t d2 = m / d;
            if (d2 != d) {
                seq_t d2_sq = (seq_t)d2 * (seq_t)d2;
                sum += d2_sq;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n >= 1 guaranteed by OEIS offset */
    seq_t s1 = sigma2(n);
    seq_t s2 = sigma2(n + (idx_t)1);
    return s1 + s2;
}
