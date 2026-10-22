/*
 * A005914  Number of points on surface of hexagonal prism: 12*n^2 + 2 for n >
 *          0 (coordination sequence for W(2)).
 * https://oeis.org/A005914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 12*n^2 + 2 for n > 0; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t n_seq = (seq_t)n;
    return 12 * n_seq * n_seq + 2;
}
