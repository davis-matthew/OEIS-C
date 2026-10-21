/*
 * A096952  Numerators of upper bounds for Lagrange remainder in Taylor's
 *          expansion of log((1+x)/(1-x)) for x=1/3, multiplied by 6/5.
 * https://oeis.org/A096952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (2^(2*n+1) + 3^(2*n+1)) / gcd(2^(2*n+1)+3^(2*n+1),
 *            5*(2*n+1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent m = 2*n + 1 */
    seq_t m = (seq_t)(2 * n + 1);

    /* compute 2^m and 3^m */
    seq_t pow2 = big_pow((seq_t)2, m);
    seq_t pow3 = big_pow((seq_t)3, m);

    /* numerator candidate */
    seq_t p = pow2 + pow3;

    /* divisor 5*(2*n+1) */
    seq_t d = (seq_t)5 * (seq_t)(2 * n + 1);

    /* greatest common divisor */
    seq_t g = gcd(p, d);

    /* reduced numerator */
    return p / g;
}
