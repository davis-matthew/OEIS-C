/*
 * A000566  Heptagonal numbers (or 7-gonal numbers): n*(5*n-3)/2.
 * https://oeis.org/A000566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(5*n-3)/2
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return m * (5 * m - 3) / 2;
}
