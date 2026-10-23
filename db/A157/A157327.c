/*
 * A157327  Egyptian fraction expansion for Pi/4 = arctan(1/2) + arctan(1/3)
 *          (Hutton 1776).
 * https://oeis.org/A157327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = (-1)^{floor(n/2)} * (2*floor(n/2)+1) * b^{2*floor(n/2)+1}
 *            where b = 2 if n is even, b = 3 if n is odd
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 36, i128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* integer division n/2 gives k = floor(n/2) */
    idx_t k = n / 2;

    /* coefficient (2k+1) and exponent (2k+1) are equal */
    seq_t coeff = (seq_t)(2 * k + 1);
    seq_t exp   = coeff;          /* same value */

    /* choose base: 2 for even n, 3 for odd n */
    seq_t base = ((n & 1) == 0) ? (seq_t)2 : (seq_t)3;

    /* compute base^exp as a seq_t */
    seq_t pow_val = big_pow(base, exp);

    /* combine: coeff * base^exp */
    seq_t result = coeff * pow_val;

    /* apply alternating sign: (-1)^k */
    if (k & 1)  /* k is odd */
        result = -result;

    return result;
}
