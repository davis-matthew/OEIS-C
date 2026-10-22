/*
 * A002717  a(n) = floor(n(n+2)(2n+1)/8).
 * https://oeis.org/A002717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*(n+2)*(2n+1)/8)
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (nn * (nn + 2) * (2*nn + 1)) / 8;
}
