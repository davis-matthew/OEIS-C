/*
 * A159921  Numerator of Hermite(n, 18/23).
 * https://oeis.org/A159921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n, 18/23) Recurrence: a(0)=1, a(1)=36
 *            a(n+1) = 36*a(n) - 1058*n*a(n-1) (n >= 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;

    seq_t prev = (seq_t)1;          /* a(0) */
    seq_t curr = (seq_t)36;         /* a(1) */
    seq_t next;

    for (idx_t k = 1; k < n; ++k) { /* compute a(k+1) */
        /* 1058*k = 2*k*529 */
        seq_t coeff = (seq_t)1058 * (seq_t)k;
        next = (seq_t)36 * curr - coeff * prev;
        prev = curr;
        curr = next;
    }

    return curr;   /* curr holds a(n) */
}
