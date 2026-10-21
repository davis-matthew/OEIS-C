/*
 * A078047  Expansion of (1-x)/(1+x+x^2+2*x^3).
 * https://oeis.org/A078047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n-1)+a(n-2)+2*a(n-3)) for n >= 3 with initial values
 *            a(0) = 1, a(1) = -2, a(2) = 1. This is derived from the
 *            generating function G(x) = (1 - x)/(1 + x + x^2 + 2x^3)
 * Range:     i8 n <= 17, i16 n <= 34, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)1;

    /* Use three rolling variables to compute the sequence iteratively. */
    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)-2;  /* a(n-2)  */
    seq_t a2 = (seq_t)1;   /* a(n-1)  */

    seq_t ai;
    for (idx_t i = 3; i <= n; ++i)
    {
        ai = -a2 - a1 - (seq_t)2 * a0;  /* a(i) = -(a(i-1)+a(i-2)+2*a(i-3)) */
        a0 = a1;        /* shift window: a(i-3) becomes previous a(i-2) */
        a1 = a2;        /* a(i-2) becomes previous a(i-1) */
        a2 = ai;        /* a(i-1) becomes current a(i) */
    }
    return a2;          /* a(n) is stored in a2 after loop */
}
