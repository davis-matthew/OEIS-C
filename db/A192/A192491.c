/*
 * A192491  Molecular topological indices of the complete tripartite graphs
 *          K_{n,n,n}.
 * https://oeis.org/A192491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2*(3*n-1)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = (seq_t)12 * N * N * (((seq_t)3 * N) - (seq_t)1);
    return term;
}
