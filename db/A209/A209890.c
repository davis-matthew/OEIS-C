/*
 * A209890  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          two distinct values, and new values 0..2 introduced in row major
 *          order.
 * https://oeis.org/A209890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) (n≥3), a(1)=7, a(2)=34
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A209890.
   The function works for any order of calls and for any n≥1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)7;
    if (n == (idx_t)2) return (seq_t)34;

    seq_t prev2 = (seq_t)7;   /* a(1) */
    seq_t prev1 = (seq_t)34;  /* a(2) */
    seq_t curr;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        curr = (seq_t)4 * prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
