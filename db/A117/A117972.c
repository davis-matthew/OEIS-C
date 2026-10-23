/*
 * A117972  Numerator of zeta'(-2n), n >= 0.
 * https://oeis.org/A117972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (-1)^n * odd_part( (2n)! ) This is the numerator of
 *            ((-1)^n * (2n)! / 2^(2n+1))
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t two_n = n + n;          /* 2*n */
    seq_t res = 1;

    for (idx_t i = 1; i <= two_n; ++i) {
        seq_t t = (seq_t)i;             /* copy current integer */
        /* strip all factors of 2 from t */
        while ((t & 1) == 0) t >>= 1;
        res *= t;                       /* multiply the odd part */
    }

    if (n & 1)                          /* apply (-1)^n */
        res = -res;

    return res;
}
