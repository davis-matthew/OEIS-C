/*
 * A192793  Molecular topological indices of the crossed prism graphs.
 * https://oeis.org/A192793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n*(2*n*n + 7)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Compute 12 * N * (2*N^2 + 7) */
    seq_t term = (seq_t)12 * N * (((seq_t)2 * N * N) + (seq_t)7);
    return term;
}
