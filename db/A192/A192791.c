/*
 * A192791  Molecular topological index of the n-antiprism graph.
 * https://oeis.org/A192791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n^2 + n + 8)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                      /* convert index to seq_t */
    return (seq_t)4 * tn * (tn * tn + tn + (seq_t)8);
}
