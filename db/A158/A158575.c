/*
 * A158575  a(n) = 32*n^2 + 1.
 * https://oeis.org/A158575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nm = (seq_t)n;
    return (seq_t)32 * nm * nm + (seq_t)1;
}
