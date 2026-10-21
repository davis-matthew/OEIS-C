/*
 * A223211  3 X 3 X 3 triangular graph coloring a rectangular array: number of
 *          n X 1 0..5 arrays where 0..5 label nodes of a graph with edges 0,1
 *          0,2 1,2 1,3 1,4 2,4 3,4 2,5 4,5 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) with a(1) = 6, a(2) = 18
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)18;

    /* Iteratively apply the recurrence. */
    seq_t prev2 = (seq_t)6;   /* a(1)   */
    seq_t prev1 = (seq_t)18;  /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 4*a(i-2)  */
        cur = prev1 * (seq_t)2 + prev2 * (seq_t)4;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
