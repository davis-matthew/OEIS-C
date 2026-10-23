/*
 * A159516  Numerator of Hermite(n, 7/15).
 * https://oeis.org/A159516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15^n * Hermite(n,7/15) Recurrence: a(0)=1, a(1)=14 For
 *            n>=2: a(n) = 14*a(n-1) - 450*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)14;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)14;  /* a(1) */
    seq_t a_curr  = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 14*a(i-1) - 450*(i-1)*a(i-2) */
        a_curr = 14 * a_prev1 - 450 * (seq_t)(i - 1) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
