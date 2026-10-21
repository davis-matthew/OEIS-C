/*
 * A221172  a(0)=-2, a(1)=3; thereafter a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A221172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = -2, a(1) = 3
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-2;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)-2;
    seq_t prev1 = (seq_t)3;
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)2 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
