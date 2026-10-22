/*
 * A006124  a(n) = 3 + n/2 + 7*n^2/2.
 * https://oeis.org/A006124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + n/2 + 7*n^2/2
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return (6 + n_seq + 7 * n_seq * n_seq) / 2;
}
