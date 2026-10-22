/*
 * A007979  Expansion of (1+x^2)(1+x^4)/((1-x)^2*(1-x^2)*(1-x^3)).
 * https://oeis.org/A007979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + 24*n + 18) / 18)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t numerator = 2 * n3 + 3 * n2 + 24 * n_seq + 18;
    return numerator / 18;
}
