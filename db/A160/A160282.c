/*
 * A160282  Numerator of Hermite(n, 20/29).
 * https://oeis.org/A160282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 40*a(n-1) - 1682*(n-1)*a(n-2)
 *           a(0) = 1
 *           a(1) = 40
 *
 * This implements the OEIS sequence A160282: Numerator of Hermite(n, 20/29).
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)40;

    /* iterative recurrence */
    seq_t a0 = (seq_t)1;          /* a(n-2) */
    seq_t a1 = (seq_t)40;         /* a(n-1) */
    seq_t a2;                     /* a(n) */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* term: 1682*(k-1) * a(n-2) */
        seq_t term = (seq_t)1682 * (seq_t)(k-1);
        a2 = (seq_t)40 * a1 - term * a0;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
