/*
 * A087169  Expansion of (1 + 3*x)/(1 + 5*x + 9*x^2).
 * https://oeis.org/A087169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -5*a(n-1) - 9*a(n-2), a(0)=1, a(1)=-2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;

    /* Iteratively compute the recurrence using only seq_t arithmetic. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-2;  /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        /* next = - (5 * a1 + 9 * a0) */
        next = - ((seq_t)5 * a1 + (seq_t)9 * a0);
        a0 = a1;
        a1 = next;
    }

    return a1;
}
