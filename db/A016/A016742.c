/*
 * A016742  Even squares: a(n) = (2*n)^2.
 * https://oeis.org/A016742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^2 = 4*n^2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 900, u64 n <= 900,
 *            u128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return x * x * (seq_t)4;
}
