/*
 * A017686  Denominator of sum of -11th powers of divisors of n.
 * https://oeis.org/A017686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{d|n} 1/d^11 = a(n) = n^11 / gcd(n^11,
 *            sigma_11(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma_11(n) = Σ_{d|n} d^11 */

#include <stdint.h>
#include "../utils/sigma_k.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute n^11 */
    seq_t n_pow = (seq_t)n;
    for (idx_t i = 1; i < 11; ++i) {
        n_pow *= (seq_t)n;
    }

    /* compute sigma_11(n) */
    seq_t sigma = sigma_k(n, 11);

    /* gcd of n^11 and sigma_11(n) */
    seq_t g = gcd(n_pow, sigma);

    /* denominator after reducing the fraction */
    return n_pow / g;
}
