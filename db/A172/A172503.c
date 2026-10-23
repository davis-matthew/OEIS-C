/*
 * A172503  a(n) = numerator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form (n).(n)(n)(n)... with period
 *          (n).
 * https://oeis.org/A172503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n * 10^d / gcd(n*10^d , 10^d – 1) * where d = number of
 *            decimal digits of n. The fraction * represents the decimal
 *            n.(n)(n)(n)... with period n
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* number of decimal digits of n (n ≥ 1) */
    int d = digits(n);

    /* compute 10^d as seq_t */
    seq_t pow10 = 1;
    for (int i = 0; i < d; ++i) {
        pow10 *= (seq_t)10;
    }

    /* denominator of the repeating part: 10^d – 1 */
    seq_t denom = pow10 - (seq_t)1;

    /* numerator before reduction: n * 10^d */
    seq_t numerator = (seq_t)n * pow10;

    /* reduce by gcd */
    seq_t g = gcd(numerator, denom);

    /* return reduced numerator */
    return numerator / g;
}
