/*
 * A007980  Expansion of (1+x^2)/((1-x)^2*(1-x^3)).
 * https://oeis.org/A007980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((n+1)*(n+2)/3)
 * Range:     u8 n <= 26, u16 n <= 441, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nm = (seq_t)n;
    return ((nm + 1) * (nm + 2) + 2) / 3;
}
