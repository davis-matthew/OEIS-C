/*
 * A223557  Petersen graph (3,1) coloring a rectangular array: number of 2 X n
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal or antidiagonal neighbor moves along an edge of this
 *          graph, with the array starting at 0.
 * https://oeis.org/A223557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2) for n>3 a(1) = 6, a(2) = 27, a(3) =
 *            171
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three known initial values explicitly. */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)171;

    /* For n > 3 compute iteratively using the recurrence.
       No dynamic allocation is required. */
    seq_t prev2 = (seq_t)27;    /* a(n-2)   */
    seq_t prev1 = (seq_t)171;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur =  (seq_t)7 * prev1 - (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
