/*
 * A017665  Numerator of sum of reciprocals of divisors of n.
 * https://oeis.org/A017665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   a(n) = sigma(n)/gcd(n, sigma(n)) (numerator of sum of
 *            reciprocals of divisors of n)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t g     = gcd((seq_t)n, sigma);
    return sigma / g;
}
