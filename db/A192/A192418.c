/*
 * A192418  Molecular topological indices of the complete bipartite graphs
 *          K_{n,n}.
 * https://oeis.org/A192418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2*(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* converted index */
    seq_t sq = nn * nn;           /* n^2 */
    seq_t part = (2 * nn - 1);    /* 2*n - 1 */
    return sq * part * 4;         /* 4*n^2*(2*n-1) */
}
