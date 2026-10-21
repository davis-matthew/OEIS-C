/*
 * A223291  4-loop graph coloring a rectangular array: number of n X 2 0..8
 *          arrays where 0..8 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 0,5 5,6 6,0 0,7 7,8 8,0 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 6*a(n-2); a(1)=24, a(2)=168
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)168;

    seq_t a_prevprev = (seq_t)24;
    seq_t a_prev     = (seq_t)168;
    seq_t a_curr     = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)9 * a_prev - (seq_t)6 * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_curr;
}
