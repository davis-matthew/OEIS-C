/*
 * A228597  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to a benzenoid consisting of a linear chain of n
 *          hexagons.
 * https://oeis.org/A228597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 96*n^2 + 16*n + 29
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to the sequence type */
    return 96 * N * N + 16 * N + 29;
}
