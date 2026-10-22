/*
 * A002593  a(n) = n^2*(2*n^2 - 1); also Sum_{k=0..n-1} (2k+1)^3.
 * https://oeis.org/A002593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^2*(2*n^2 - 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    return n_sq * (2 * n_sq - 1);
}
