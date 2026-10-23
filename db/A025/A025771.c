/*
 * A025771  Expansion of 1/((1-x)*(1-x^3)*(1-x^11)).
 * https://oeis.org/A025771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 15*n + 78)/66)
 * Range:     u8 n <= 122, u16 n <= 2072, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    return (nn * nn + (seq_t)15 * nn + (seq_t)78) / (seq_t)66;
}
