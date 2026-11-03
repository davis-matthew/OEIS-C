/*
 * A098098  a(n) = sigma(6*n+5)/6.
 * https://oeis.org/A098098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(6*n+5)/6
 * Range:     u8 n <= 34, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9556 (2026-11-01)
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
    /* compute m = 6*n + 5 (all in idx_t) */
    idx_t m = n * 6ULL + 5ULL;
    /* sigma_of returns the sum of divisors of m */
    seq_t sum = sigma_of(m);
    /* division by 6 is exact for all m ≡ 5 (mod 6) */
    return sum / 6;
}
