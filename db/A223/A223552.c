/*
 * A223552  Petersen graph (3,1) coloring a rectangular array: number of n X 4
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal or antidiagonal neighbor moves along an edge of this
 *          graph, with the array starting at 0.
 * https://oeis.org/A223552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=27, a(2)=1089, a(n)=41*a(n-1)-16*a(n-2) for n≥3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined below offset, return 0 */
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)1089;

    seq_t prev2 = (seq_t)27;   /* a(1) */
    seq_t prev1 = (seq_t)1089; /* a(2) */
    seq_t cur   = prev1;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)41 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
