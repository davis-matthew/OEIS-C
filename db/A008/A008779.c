/*
 * A008779  Number of n-dimensional partitions of 5.
 * https://oeis.org/A008779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^3 + 21*n^2 + 38*n + 24)/24
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;               /* n^2 */
    seq_t n3   = n2 * n_seq;                  /* n^3 */
    seq_t poly = n3 + (seq_t)21 * n2 + (seq_t)38 * n_seq + (seq_t)24;
    seq_t prod = (n_seq + (seq_t)1) * poly;
    return prod / (seq_t)24;
}
