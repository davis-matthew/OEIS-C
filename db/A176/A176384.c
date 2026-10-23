/*
 * A176384  y-values in the solution to x^2-73*y^2=1.
 * https://oeis.org/A176384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4562498*a(n-1) - a(n-2) with a(1) = 0, a(2) = 267000
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)267000;

    /* Iterative recurrence using only two previous values */
    seq_t prev2 = (seq_t)0;        /* a(n-2) */
    seq_t prev1 = (seq_t)267000;  /* a(n-1) */
    const seq_t MUL = (seq_t)4562498;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = MUL * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
