/*
 * A212558  a(n) = ((n - s)^2 mod (n + s)) - ((n + s)^2 mod (n - s)), where s
 *          is the sum of the decimal digits of n.
 * https://oeis.org/A212558
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = ((n - s)^2 mod (n + s)) - ((n + s)^2 mod (n - s)), where
 *            s = digit_sum(n)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t */
    seq_t ns = (seq_t)n;

    /* Sum of decimal digits of n */
    seq_t s = digit_sum(ns);

    /* (n - s) and (n + s) */
    seq_t m1 = ns - s;   /* n - s */
    seq_t m2 = ns + s;   /* n + s */

    /* (n - s)^2 mod (n + s) */
    seq_t r1 = (m1 * m1) % m2;

    /* (n + s)^2 mod (n - s) */
    seq_t r2 = (m2 * m2) % m1;

    /* Final result */
    return r1 - r2;
}
