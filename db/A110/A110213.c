/*
 * A110213  a(n+3) = 6*a(n) - 5*a(n+2), a(0) = 1, a(1) = -7, a(2) = 35.
 * https://oeis.org/A110213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 6*a(n) - 5*a(n+2),
   a(0) = 1, a(1) = -7, a(2) = 35 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-7;
    if (n == 2) return (seq_t)35;

    /* Three sliding values: t0 = a(n-3), t1 = a(n-2), t2 = a(n-1) */
    seq_t t0 = (seq_t)1;    /* a(0) */
    seq_t t1 = (seq_t)-7;   /* a(1) */
    seq_t t2 = (seq_t)35;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = 6 * t0 - 5 * t2;  /* a(i) = 6*a(i-3) - 5*a(i-1) */
        /* Shift one step forward for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = cur;
    }
    return t2;
}
