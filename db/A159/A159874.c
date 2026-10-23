/*
 * A159874  Numerator of Hermite(n, 10/23).
 * https://oeis.org/A159874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n,10/23). Recurrence: a(0)=1, a(1)=20,
 *            a(n+1) = 20*a(n) - 1058*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)20;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)20;  /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        /* a(i+1) = 20*a(i) - 1058*i*a(i-1) */
        seq_t term = (seq_t)(20 * prev - 1058 * (i) * prevprev);
        prevprev = prev;
        prev     = term;
    }
    return prev;   /* a(n) */
}
