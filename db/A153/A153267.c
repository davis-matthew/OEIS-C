/*
 * A153267  a(n) = -4*a(n-3) + 11*a(n-2) - a(n-1), a(0) = -5, a(1) = 39, a(2)
 *          = -110.
 * https://oeis.org/A153267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-3) + 11*a(n-2) - a(n-1) initial values: a(0) = -5,
 *            a(1) = 39, a(2) = -110
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 14, i64 n <= 30, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)(-5);
    if (n == 1) return (seq_t)(39);
    if (n == 2) return (seq_t)(-110);

    /* We keep the last three terms to compute the next one. */
    seq_t a0 = (seq_t)(-5);   /* a(n-3) */
    seq_t a1 = (seq_t)(39);   /* a(n-2) */
    seq_t a2 = (seq_t)(-110); /* a(n-1) */
    seq_t a3 = 0;

    /* Iterate up to the requested index. */
    for (idx_t i = 3; i <= n; ++i) {
        a3 = -4 * a0 + 11 * a1 - a2;
        /* Shift the window: a(n-3) becomes previous a(n-2), etc. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a3;
}
