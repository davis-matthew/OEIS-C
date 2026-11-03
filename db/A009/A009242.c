/*
 * A009242  a(n) = lcm(n, sigma(n)).
 * https://oeis.org/A009242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n, sigma(n))
 * Range:     u8 n <= 15, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9151 (2026-11-01)
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
    if (n == 0) return 0;               /* offset is 1, undefined for n=0 */
    seq_t n_t   = (seq_t)n;
    seq_t sigma = sigma_of(n);          /* Σ(d | n) d */
    seq_t g     = gcd(n_t, sigma);      /* gcd(n, sigma(n)) */
    return (n_t / g) * sigma;           /* lcm = n/g * sigma */
}
