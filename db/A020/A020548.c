/*
 * A020548  3rd Euler polynomial evaluated at x=n! (multiplied by 4).
 * https://oeis.org/A020548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 181 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*(n!)^3 - 6*(n!)^2 + 1
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t f     = factorial(n);      /* n! */
    seq_t f2    = f * f;             /* (n!)^2 */
    seq_t f3    = f2 * f;            /* (n!)^3 */

    seq_t result = 4 * f3;           /* 4*(n!)^3 */
    result = result - 6 * f2;        /* -6*(n!)^2 */
    result = result + 1;             /* +1 */
    return result;
}
