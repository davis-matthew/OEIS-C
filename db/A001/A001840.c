/*
 * A001840  Expansion of g.f. x/((1 - x)^2*(1 - x^3)).
 * https://oeis.org/A001840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n+1)*(n+2)/6)
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 1;
    seq_t k = (seq_t)n + 2;
    return (m * k) / 6;
}
