/*
 * A178219  a(0)=a(1)=a(2)=0, a(3)=1; thereafter a(n) = 4a(n-3)-5a(n-4).
 * https://oeis.org/A178219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 39, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=0, a(3)=1; thereafter
   a(n) = 4*a(n-3) - 5*a(n-4)   for n ≥ 4.
   This implementation keeps only the last four values
   and works for any idx_t and seq_t (sized or arbitrary
   precision) with the allowed operators. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the known initial values directly */
    if (n == 0u || n == 1u || n == 2u)
        return (seq_t)0;
    if (n == 3u)
        return (seq_t)1;

    /* A ring buffer of the last four computed terms */
    seq_t buf[4];
    buf[0] = (seq_t)0;      /* a(0) */
    buf[1] = (seq_t)0;      /* a(1) */
    buf[2] = (seq_t)0;      /* a(2) */
    buf[3] = (seq_t)1;      /* a(3) */

    /* compute from 4 up to n inclusive */
    for (idx_t i = 4u; i <= n; ++i) {
        /* indices modulo 4 */
        idx_t idx   = i & 3u;             /* current position */
        idx_t idx3  = (i - 3u) & 3u;      /* position of a(i-3) */
        idx_t idx4  = (i - 4u) & 3u;      /* position of a(i-4) */

        buf[idx] = (seq_t)4 * buf[idx3] - (seq_t)5 * buf[idx4];
    }

    /* a(n) is stored at n mod 4 */
    return buf[n & 3u];
}
