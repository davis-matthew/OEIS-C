/*
 * A134185  Hankel transform of a transform of the central binomial
 *          coefficients A001405.
 * https://oeis.org/A134185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((1/2 - 1/sqrt(2))*cos(3*Pi*n/4) + (1/2 +
 *            1/sqrt(2))*cos(Pi*n/4))*2^ceil(binomial(n+1,2)/2)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified:  Let f(n) = 1 if n mod 8 = 0,1,7; 0 if n mod 8 = 2,6; -1 if n mod 8 = 3,4,5.  */
/*  Let e(n) = ceil((n^2 + n)/4) = (n*n + n + 3)/4.  Then a(n) = f(n) * 2^e(n). */

#include "../utils/pow2.h"

/* Computes OEIS sequence A134185 */
seq_t a(idx_t n)
{
    /* Determine factor f(n) based on n mod 8 */
    idx_t r = n & 7;            /* equivalent to n % 8 */
    seq_t sign;
    if (r == 2 || r == 6)
        return 0;              /* f(n) = 0 */

    if (r == 3 || r == 4 || r == 5)
        sign = -1;              /* f(n) = -1 */
    else
        sign = 1;               /* f(n) = 1  (r == 0,1,7) */

    /* Compute exponent e(n) = ceil((n^2 + n)/4) = (n*n + n + 3)/4 */
    idx_t e = (n * n + n + 3) / 4;

    /* Compute 2^e */
    seq_t p = pow2(e);

    /* Apply sign */
    if (sign < 0)
        p = -p;

    return p;
}
