/*
 * A009242  a(n) = lcm(n, sigma(n)).
 * https://oeis.org/A009242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n, sigma(n))
 * Range:     u8 n <= 15, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, undefined for n=0 */
    seq_t n_t   = (seq_t)n;
    seq_t sigma = sigma_of(n);          /* Σ(d | n) d */
    seq_t g     = gcd(n_t, sigma);      /* gcd(n, sigma(n)) */
    return (n_t / g) * sigma;           /* lcm = n/g * sigma */
}
