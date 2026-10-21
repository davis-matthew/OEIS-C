/*
 * A053567  Stirling numbers of first kind, s(n+5, n).
 * https://oeis.org/A053567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n*binomial(n+5,6)*binomial(n+5,2)*(3*n^2 + 23*n +
 *            38)/8
 * Range:     i8 none, i16 n <= 3, i32 n <= 16, i64 n <= 176, i128 n <= 200,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Denominator 11520 = 6! * 2 * 8 */
    seq_t denom = 11520;
    seq_t result = 1;

    seq_t factors[9];
    factors[0] = nn + 5;
    factors[1] = nn + 5;
    factors[2] = nn + 4;
    factors[3] = nn + 4;
    factors[4] = nn + 3;
    factors[5] = nn + 2;
    factors[6] = nn + 1;
    factors[7] = nn;
    /* polynomial 3*n^2 + 23*n + 38 */
    seq_t poly = (seq_t)3 * nn * nn + (seq_t)23 * nn + (seq_t)38;
    factors[8] = poly;

    for (int i = 0; i < 9; ++i)
    {
        seq_t g = gcd(factors[i], denom);
        denom /= g;
        factors[i] /= g;
        result *= factors[i];
    }

    /* any remainder denominator should be 1, but guard just in case */
    if (denom != 1)
        result /= denom;

    if ((n & 1) != 0)
        result = -result;

    return result;
}
