/*
 * A145593  Inverse binomial transform of A144472.
 * https://oeis.org/A145593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n * (10 * 2^n - 27) + 11) / 6 Recurrence: a(0) = -1
 *            a(n+1) = -( 2 * a(n) + (-1)^n * ((1-(-1)^n) * 5 + 1) )
 *            Implemented with an iterative recurrence that uses only 64‑bit
 *            or arbitrary‑precision arithmetic and never overflows for values
 *            that fit in seq_t
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = -1;                    /* a(0) */
    if (n == 0) return res;
    for (idx_t i = 0; i < n; ++i)      /* compute a(1) … a(n) */
    {
        /* sign = (-1)^i : 1 if i even, -1 if i odd */
        seq_t sign = ((i & (idx_t)1) == 0) ? 1 : -1;
        /* delta = -1 if i even, 10 if i odd (from the formula) */
        seq_t delta = ((i & (idx_t)1) == 0) ? -1 : 10;
        res = -( res * 2 + sign * delta );
    }
    return res;
}
