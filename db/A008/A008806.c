/*
 * A008806  Expansion of (1+x^3)/((1-x^2)^2*(1-x^3)).
 * https://oeis.org/A008806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n^2 + 24*n + 61 + 27*(-1)^n) / 72)
 * Range:     u8 n <= 3, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* For even n add 88, for odd n add 34 */
    seq_t add = (n & 1) ? (seq_t)34 : (seq_t)88;
    /* numerator = 6*n*n + 24*n + add */
    seq_t numerator = 6 * nn * nn + 24 * nn + add;
    return numerator / (seq_t)72;
}
