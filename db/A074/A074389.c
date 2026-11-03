/*
 * A074389  a(n) = gcd(n, sigma(n), phi(n)).
 * https://oeis.org/A074389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(n), phi(n))
 * Range:     u8 n <= 1145, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9800 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

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
    /* Return 0 for n == 0, though a(1) is the first term of interest. */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t sn = sigma_of(n);       /* σ(n) */
    seq_t pn = totient(n);            /* φ(n) */
    seq_t g  = gcd((seq_t)n, sn); /* gcd(n, σ(n)) */
    g       = gcd(g, pn);         /* gcd(g, φ(n)) */
    return g;
}
