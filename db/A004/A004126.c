/*
 * A004126  a(n) = n*(7*n^2 - 1)/6.
 * https://oeis.org/A004126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n^2 - 1)/6
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 1059, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t temp = n_seq * (7 * n_seq * n_seq - 1);
    return temp / 6;
}
