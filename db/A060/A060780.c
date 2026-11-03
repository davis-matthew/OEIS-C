/*
 * A060780  a(n) = gcd(sigma(n+1), sigma(n)), where sigma = A000203.
 * https://oeis.org/A060780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n+1), sigma(n))
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 108 terms through n = 845 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
    /* Compute the divisor sums of n and n+1 */
    seq_t sigma_n  = sigma_of(n);
    seq_t sigma_np1 = sigma_of(n + 1);

    /* Return their greatest common divisor */
    return gcd(sigma_n, sigma_np1);
}
