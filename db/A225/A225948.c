/*
 * A225948  a(0) = -1; for n>0, a(n) = numerator(1/4 - 4/n^2).
 * https://oeis.org/A225948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator(1/4 - 4/n^2)
 * Range:     i8 n <= 5, i16 n <= 181, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = -1; for n > 0, a(n) = (n^2-16) / (4*n^2) reduced to lowest terms. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return -1;

    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                 /* n^2 */
    seq_t num = sq - 16;                /* numerator: n^2 - 16 */
    seq_t denom = (seq_t)4 * sq;        /* denominator: 4 * n^2 */

    seq_t g = gcd(num, denom);          /* greatest common divisor of numerator and denominator */
    return num / g;                     /* reduce to lowest terms and return the numerator */
}
