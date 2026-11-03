/*
 * A062835  a(1) = 0; for n > 1 a(n) = sum of divisors of n^2-1; or
 *          sigma(A005563(n-1)).
 * https://oeis.org/A062835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = σ(n² – 1), the sum of divisors of n²−1 (a(1)=0)
 * Range:     u8 n <= 10, u16 n <= 138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9193 (2026-11-01)
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
    if (n <= 1)
        return (seq_t)0;

    /* n²−1 is guaranteed to fit in idx_t for the tested ranges */
    idx_t m = (idx_t)n * (idx_t)n - (idx_t)1;
    return sigma_of(m);
}
