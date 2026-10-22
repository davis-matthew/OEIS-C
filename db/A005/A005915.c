/*
 * A005915  Hexagonal prism numbers: a(n) = (n + 1)*(3*n^2 + 3*n + 1).
 * https://oeis.org/A005915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (n+1)*(3*n^2 + 3*n + 1) = 3*n^3 + 6*n^2 + 4*n + 1
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return ((3*x + 6)*x + 4)*x + 1;
}
