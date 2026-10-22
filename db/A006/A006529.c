/*
 * A006529  a(n) = (25*n^4-120*n^3+209*n^2-108*n)/6.
 * https://oeis.org/A006529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n^4 - 120*n^3 + 209*n^2 - 108*n) / 6
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    return (25*x4 - 120*x3 + 209*x2 - 108*x) / 6;
}
