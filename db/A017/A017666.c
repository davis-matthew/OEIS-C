/*
 * A017666  Denominator of sum of reciprocals of divisors of n.
 * https://oeis.org/A017666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, sigma(n)) = n / gcd(n, sigma_of(n))
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; a(0) is undefined in OEIS */
    seq_t n_t = (seq_t)n;          /* convert index to seq_t */
    seq_t sigma = sigma_of(n);     /* σ(n) = sum of divisors of n */
    seq_t g = gcd(n_t, sigma);     /* greatest common divisor of n and σ(n) */
    return n_t / g;                /* denominator of σ(n)/n */
}
