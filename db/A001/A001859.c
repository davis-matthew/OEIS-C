/*
 * A001859  Triangular numbers plus quarter-squares: n*(n+1)/2 +
 *          floor((n+1)^2/4) (i.e., A000217(n) + A002620(n+1)).
 * https://oeis.org/A001859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n + floor((3n^2 + 1) / 4)
 * Range:     u8 n <= 9, u16 n <= 293, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t three_n_sq = 3 * nn * nn;
    return nn + (three_n_sq + 1) / 4;
}
