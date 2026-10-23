/*
 * A017708  Denominator of sum of -22nd powers of divisors of n.
 * https://oeis.org/A017708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^22 / gcd( sigma_22(n), n^22 )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow, gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_k(n, 22);     /* sum of d^22 over divisors of n   */
    seq_t n_pow = big_pow((seq_t)n, 22); /* n^22                               */
    seq_t g = gcd(sigma, n_pow);     /* common factor between sigma and n^22 */
    return n_pow / g;                /* reduced denominator               */
}
