/*
 * A001249  Squares of tetrahedral numbers: a(n) = binomial(n+3,n)^2.
 * https://oeis.org/A001249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3, 3)^2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n + 1;
    seq_t binom = (nn * (nn + 1) * (nn + 2)) / 6;
    return binom * binom;
}
