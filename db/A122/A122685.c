/*
 * A122685  a(n) = n! except that a(2) = -2 and a(2n) = 0 for n > 2.
 * https://oeis.org/A122685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! except a(2) = -2 and a(2n)=0 for n>2
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    if (n == 0)                 return (seq_t)1;
    if (n == 1)                 return (seq_t)1;
    if (n == 2)                 return (seq_t)-2;
    if ((n & 1) == 0)           return (seq_t)0;   /* even > 2 */
    return factorial(n);        /* odd n (≥3) */
}
