/*
 * A166577  Inverse binomial transform of A166517.
 * https://oeis.org/A166577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(1)=4; a(n) = -5 * (-1)^n * 2^(n-2) for n>1
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* compute 2^(n-2) */
    seq_t term = pow2(n - 2);

    /* sign factor: -5 for even n, +5 for odd n */
    if (n & 1)
        term *= (seq_t)5;
    else
        term *= (seq_t)-5;

    return term;
}
