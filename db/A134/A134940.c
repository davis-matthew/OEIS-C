/*
 * A134940  Define f(n) by e(n+1) = e(n) + 3^{n+1} - 1 + 2*f(n), where the
 *          rational numbers e(n) are defined in A134939; then a(n) is the
 *          numerator of f(n).
 * https://oeis.org/A134940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (6*3^n-1)*(5^n-3^n)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 5^n */
    seq_t three = big_pow((seq_t)3, n);
    seq_t five  = big_pow((seq_t)5, n);

    /* (6*3^n - 1) * (5^n - 3^n) / 2 */
    seq_t t1 = (seq_t)6 * three - (seq_t)1;
    seq_t t2 = five - three;
    seq_t mul = t1 * t2;
    seq_t res = mul / (seq_t)2;

    return res;
}
