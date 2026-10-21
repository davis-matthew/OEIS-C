/*
 * A086601  Triangular numbers + 1 squared.
 * https://oeis.org/A086601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n*(n+1))/2 + 1)^2 = (triangular(n)+1)^2
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;  /* triangular number */
    seq_t v = t + 1;
    return v * v;
}
