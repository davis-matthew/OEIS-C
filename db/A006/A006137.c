/*
 * A006137  a(n) = 1 + n/2 + 9*n^2/2.
 * https://oeis.org/A006137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 + n + 9*n^2) / 2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    return (2 + n_seq + 9 * n_sq) / 2;
}
