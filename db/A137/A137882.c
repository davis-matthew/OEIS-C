/*
 * A137882  Number of (directed) Hamiltonian paths in the n-ladder graph.
 * https://oeis.org/A137882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n*n - n + 2) for n > 1, a(1) = 2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;           /* n^2 in seq_t */
    seq_t temp = sq - sn + (seq_t)2;
    return (seq_t)2 * temp;       /* 2*(n^2 - n + 2) */
}
