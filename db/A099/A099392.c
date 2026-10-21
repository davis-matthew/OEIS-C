/*
 * A099392  a(n) = floor((n^2 - 2*n + 3)/2).
 * https://oeis.org/A099392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 - 2*n + 3)/2)
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* convert index to sequence type */
    return (N * N - (seq_t)2 * N + (seq_t)3) / (seq_t)2;
}
