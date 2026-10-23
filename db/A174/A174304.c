/*
 * A174304  Values of the family of polynomials y=x^n-1 at point x =
 *          discriminant of y.
 * https://oeis.org/A174304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n*(n-1)*(n-2)/2) * n^(n^2) - 1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 3, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;               /* 1^1 - 1 = 0 */

    idx_t exp64 = n * n;         /* n^2 */
    seq_t result = big_pow((seq_t)n, (seq_t)exp64); /* n^(n^2) */

    /* Compute the sign factor (-1)^(n*(n-1)*(n-2)/2) */
    idx_t t = n;
    t *= (n - 1);
    t *= (n - 2);
    t /= 2;                     /* exact division */
    if (t & 1)
        result = -result;

    return result - 1;
}
