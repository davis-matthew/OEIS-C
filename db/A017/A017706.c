/*
 * A017706  Denominator of sum of -21st powers of divisors of n.
 * https://oeis.org/A017706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sigma_21(n) / n^21 in lowest terms
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 69,
 *            big any n
 * Helpers:   gcd, pow_uint, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute n^21 */
    seq_t n_pow = pow_uint((uint64_t)n, (idx_t)21);

    /* compute sigma_21(n) */
    seq_t sigma = sigma_k((idx_t)n, (uint8_t)21);

    /* reduce the fraction sigma / n_pow */
    seq_t g = gcd(n_pow, sigma);

    /* denominator of the reduced fraction */
    return n_pow / g;
}
