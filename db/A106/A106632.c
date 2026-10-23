/*
 * A106632  Expansion of g.f. -(1+27*x^2)/((1+3*x)*(1-2*x+9*x^2)).
 * https://oeis.org/A106632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 3*a(n-2) - 27*a(n-3), initial values: a(0) =
 *            -1, a(1) = 1, a(2) = -25
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 38, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)-25;

    seq_t a0 = (seq_t)-1;  /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)-25; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = -a2 - (seq_t)3 * a1 - (seq_t)27 * a0;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }

    return a2;
}
