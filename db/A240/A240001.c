/*
 * A240001  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 6 n^2 + 23 n)/6
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = (N * N * N + 6 * N * N + 23 * N) / 6;
    return res;
}
