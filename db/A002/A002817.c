/*
 * A002817  Doubly triangular numbers: a(n) = n*(n+1)*(n^2+n+2)/8.
 * https://oeis.org/A002817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(n^2+n+2)/8
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 429, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n_sq = n_val * n_val;
    return n_val * (n_val + 1) * (n_sq + n_val + 2) / 8;
}
