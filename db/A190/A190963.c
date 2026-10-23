/*
 * A190963  a(n) = 3*a(n-1) - 9*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 9*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 21, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)3 * prev1 - (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
