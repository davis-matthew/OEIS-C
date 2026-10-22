/*
 * A005918  Number of points on surface of square pyramid: 3*n^2 + 2 (n>0).
 * https://oeis.org/A005918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3*n^2 + 2 for n > 0
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t n_seq = (seq_t)n;
    return 3 * n_seq * n_seq + 2;
}
