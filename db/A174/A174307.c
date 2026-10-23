/*
 * A174307  Values of the family of polynomials y = x^n + x + 1 at point x =
 *          discriminant of y.
 * https://oeis.org/A174307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = D^n + D + 1, D = (-1)^(n(n-1)/2) * (n^n + (-1)^(n-1) *
 *            (n-1)^(n-1))
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 3, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined, but return 0 for safety */
    if (n == 1)
        return 3;               /* D = 1, f(1) = 3 */

    /* Compute n^n and (n-1)^(n-1) */
    seq_t S1 = big_pow((seq_t)n, (seq_t)n);
    seq_t S2 = big_pow((seq_t)(n - 1), (seq_t)(n - 1));

    /* sign1 = (-1)^(n(n-1)/2) */
    int sign1;
    switch (n & 3ULL) {
    case 2:
    case 3:
        sign1 = -1;
        break;
    default:
        sign1 = 1;
    }

    /* sign2 = (-1)^(n-1) */
    int sign2 = (n % 2 == 0) ? -1 : 1;

    /* temp = n^n + sign2 * (n-1)^(n-1) */
    seq_t temp;
    if (sign2 == -1)
        temp = S1 - S2;
    else
        temp = S1 + S2;

    /* D = sign1 * temp */
    seq_t D = (sign1 == -1) ? -temp : temp;

    /* Compute D^n */
    seq_t D_pow = big_pow(D, (seq_t)n);

    /* a(n) = D_pow + D + 1 */
    seq_t result = D_pow + D + (seq_t)1;
    return result;
}
