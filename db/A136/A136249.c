/*
 * A136249  a(n) = -a(n-1) + 4*a(n-2) + 4*a(n-3), starting with 4, -2, 1.
 * https://oeis.org/A136249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 4*a(n-2) + 4*a(n-3); a(0)=4, a(1)=-2, a(2)=1
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the initial three terms directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)1;

    /* Use a sliding window of the last three values */
    seq_t t0 = (seq_t)4;   /* a(0) */
    seq_t t1 = (seq_t)-2;  /* a(1) */
    seq_t t2 = (seq_t)1;   /* a(2) */
    seq_t tn = t2;         /* placeholder for a(n) */

    /* Compute forward up to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        tn = -t2 + (seq_t)4 * t1 + (seq_t)4 * t0;  /* a(i) = -a(i-1)+4*a(i-2)+4*a(i-3) */
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }
    return tn;
}
