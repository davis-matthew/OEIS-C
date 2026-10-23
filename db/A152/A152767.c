/*
 * A152767  3 times 10-gonal (or decagonal) numbers: a(n) = 3*n*(4*n-3).
 * https://oeis.org/A152767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(4*n-3) = 12*n^2 - 9*n
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 12 * N * N - 9 * N;
}
