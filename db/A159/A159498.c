/*
 * A159498  Numerator of Hermite(n, 6/13).
 * https://oeis.org/A159498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n,6/13) Recurrence (derived from Hermite
 *            recurrence): a(0) = 1 a(1) = 12 a(n+1) = 12*a(n) - 338*n*a(n-1)
 *            for n >= 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a_prev = (seq_t)1;     /* a(0) */
    seq_t a_curr = (seq_t)12;    /* a(1) */

    /* Compute successive terms using the recurrence */
    for (idx_t i = 1; i < n; ++i) {
        /* a_next = 12*a_curr - 338*i*a_prev */
        seq_t term = (seq_t)i * a_prev;  /* i * a_prev */
        term *= (seq_t)338;              /* 338 * i * a_prev */
        seq_t a_next = 12 * a_curr - term;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;   /* a(n) */
}
