/*
 * A223240  3-loop graph coloring a rectangular array: number of n X 1 0..6
 *          arrays where 0..6 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 0,5 5,6 6,0 and every array movement to a horizontal
 *          or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 6*a(n-2);
 * initial values: a(1) = 7, a(2) = 18
 */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (user should call with n >= 1).  */
    if (n == 1)
        return (seq_t)7;

    if (n == 2)
        return (seq_t)18;

    /* Iterate from 3 up to n using the recurrence.  */
    seq_t prev2 = (seq_t)7;   /* a(i-2) */
    seq_t prev1 = (seq_t)18;  /* a(i-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 + (seq_t)6 * prev2; /* a(i) = a(i-1) + 6*a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
