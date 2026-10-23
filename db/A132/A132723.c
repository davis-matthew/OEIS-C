/*
 * A132723  Binomial transform of A132429.
 * https://oeis.org/A132723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=3, a(1)=a(2)=4; for n>2, a(n) = 2*a(n-1) - 2*a(n-2)
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 60, i64 n <= 124,
 *            i128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1 || n == 2)
        return (seq_t)4;

    seq_t p2 = (seq_t)4; /* a(1) */
    seq_t p1 = (seq_t)4; /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)2 * p1 - (seq_t)2 * p2;
        p2 = p1;
        p1 = curr;
    }
    return p1;
}
