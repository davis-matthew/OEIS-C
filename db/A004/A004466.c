/*
 * A004466  a(n) = n*(5*n^2 - 2)/3.
 * https://oeis.org/A004466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n^2 - 2)/3
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1010, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n_seq * n2;
    seq_t numerator = 5 * n3 - 2 * n_seq;
    return numerator / 3;
}
