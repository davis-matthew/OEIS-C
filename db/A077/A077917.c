/*
 * A077917  Expansion of (1-x)^(-1)/(1+2*x-2*x^2).
 * https://oeis.org/A077917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies a(0) = 1, a(1) = -1, a(2) = 5, and for n ≥ 3:
 *            a(n) = -a(n‑1) + 4·a(n‑2) – 2·a(n‑3)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the expansion of (1‑x)⁻¹ / (1 + 2x – 2x²). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;     /* a(n-3)  */
    seq_t a1 = (seq_t)-1;    /* a(n-2)  */
    seq_t a2 = (seq_t)5;     /* a(n-1)  */
    seq_t ai = 0;

    for (idx_t i = 3; i <= n; ++i) {
        ai = -a2 + (seq_t)4 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
