/*
 * A159969  Numerator of Hermite(n, 13/24).
 * https://oeis.org/A159969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 12^n * Hermite(n, 13/24) Recurrence: a(0)=1, a(1)=13;
 *            a(n+1) = 13*a(n) - 288*n*a(n-1) for n≥1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;

    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t a_cur  = (seq_t)13;  /* a(1) */

    for (idx_t i = 1; i < n; ++i)   /* i = n-1 iterations to reach a(n) */
    {
        seq_t tmp = (seq_t)13 * a_cur - (seq_t)288 * (seq_t)i * a_prev;
        a_prev = a_cur;
        a_cur  = tmp;
    }

    return a_cur;
}
