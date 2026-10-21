/*
 * A078050  Expansion of (1-x)/(1+x+2*x^2).
 * https://oeis.org/A078050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -2, a(n) = -a(n-1) - 2*a(n-2) for n >= 2
 * Range:     i8 n <= 13, i16 n <= 28, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078050: expansion of (1 - x)/(1 + x + 2*x^2) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-2);

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)(-2);        /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = -prev1 - (seq_t)2 * prev2;   /* a(i) = -a(i-1) - 2*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
