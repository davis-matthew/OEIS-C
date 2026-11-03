/*
 * A074465  a(n) = gcd(n^2, sigma(n^2), phi(n^2)).
 * https://oeis.org/A074465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n^2, sigma(n^2), phi(n^2))
 * Range:     u8 n <= 11, u16 n <= 1931, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 9902 (2026-11-01)
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
    /* For n >= 1 (offset 1). Return 0 when n==0 for safety. */
    if (n == 0) return 0;

    idx_t sq = (idx_t)n * (idx_t)n;   /* n^2 as an idx_t value */
    seq_t sqv = (seq_t)sq;            /* n^2 as seq_t for the gcd */

    seq_t sig = sigma_of(sq);         /* sigma(n^2) */
    seq_t ph  = totient(sq);              /* phi(n^2) */

    seq_t g = gcd(gcd(sqv, sig), ph); /* gcd of the three values */

    return g;
}
