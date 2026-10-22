/*
 * A000982  a(n) = ceiling(n^2/2).
 * https://oeis.org/A000982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ceiling(n^2/2)
 * Range:     u8 n <= 15, u16 n <= 345, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;
    return (n_sq + 1) / 2;
}
