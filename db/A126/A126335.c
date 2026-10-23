/*
 * A126335  a(n) = n*(4*n^2+5*n-3)/2.
 * https://oeis.org/A126335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2+5*n-3)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t num = 4 * nn * nn + 5 * nn - (seq_t)3;
    return nn * num / (seq_t)2;
}
