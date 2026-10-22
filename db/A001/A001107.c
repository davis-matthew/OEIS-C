/*
 * A001107  10-gonal (or decagonal) numbers: a(n) = n*(4*n-3).
 * https://oeis.org/A001107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(4*n-3)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (n_seq * 4 - 3);
}
