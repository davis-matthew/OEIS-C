/*
 * A001973  Expansion of (1+x^3)/((1-x)*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A001973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 15*n^2 + 42*n + 78) / 72)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return (2 * n3 + 15 * n2 + 42 * nn + 78) / 72;
}
