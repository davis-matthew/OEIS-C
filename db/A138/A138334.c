/*
 * A138334  a(n) = C(n+11, 11)*(n+6)*(-1)^(n+1)*512/3.
 * https://oeis.org/A138334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = C(n+11, 11)*(n+6)*(-1)^(n+1)*512/3
 * Range:     i8 none, i16 n <= 1, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute C(n+11, 11) */
    seq_t res = binom(n + 11, 11);

    /* times (n+6) */
    res *= (seq_t)(n + 6);

    /* divide by 3 */
    res /= (seq_t)3;

    /* times 512 */
    res *= (seq_t)512;

    /* apply sign (-1)^(n+1) */
    if (((n + 1) & 1) != 0)
        res = -res;

    return res;
}
