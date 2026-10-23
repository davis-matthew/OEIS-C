/*
 * A159509  Numerator of Hermite(n, 5/14).
 * https://oeis.org/A159509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159509: a(n) = 7^n * Hermite(n, 5/14)
   Recurrence: a(0) = 1, a(1) = 5,
   a(n) = 5 * a(n-1) - 98 * (n-1) * a(n-2)    for n >= 2
   This follows from the Hermite recurrence:
   H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x) with x = 5/14. */

#include <stdint.h>  /* for idx_t definition */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t cur  = (seq_t)5;   /* a(1) */

    for (idx_t k = 1; k < n; ++k) {
        /* compute a(k+1) = 5*a(k) - 98*k*a(k-1) */
        seq_t term = (seq_t)98 * (seq_t)k * prev;  /* 98*k*a(k-1) */
        seq_t next = (seq_t)5 * cur - term;        /* 5*a(k) - term */
        prev = cur;
        cur  = next;
    }
    return cur;
}
