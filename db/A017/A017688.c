/*
 * A017688  Denominator of sum of -12th powers of divisors of n.
 * https://oeis.org/A017688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^12 / gcd(sigma_12(n), n^12)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1625,
 *            big any n
 * Helpers:   big_pow, seq_gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_k.h"
#include "../utils/big_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* sigma_12(n) = Σ_{d|n} d^12 */
    seq_t sigma12 = sigma_k(n, 12U);

    /* n^12 as a seq_t */
    seq_t n_pow12 = big_pow((seq_t)n, (seq_t)12U);

    /* greatest common divisor */
    seq_t g = seq_gcd(sigma12, n_pow12);

    /* denominator of σ12(n) / n^12 */
    return n_pow12 / g;
}
