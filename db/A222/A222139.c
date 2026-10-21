/*
 * A222139  Number of n X 3 0..4 arrays with no entry increasing mod 5 by 4
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=16, a(2)=676, a(n)=45*a(n-1)-116*a(n-2) */
#include <stdint.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 1)
        return (seq_t)16;
    if (n == 2)
        return (seq_t)676;

    /* For larger n compute iteratively using the recurrence. */
    seq_t prev2 = (seq_t)16;   /* a(n-2) */
    seq_t prev1 = (seq_t)676;  /* a(n-1) */
    seq_t cur   = 0;           /* a(n) */

    /* n is unsigned, so the loop condition is safe. */
    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)45 * prev1 - (seq_t)116 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
