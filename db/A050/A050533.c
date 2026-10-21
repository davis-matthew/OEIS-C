/*
 * A050533  Thickened pyramidal numbers: a(n) = 2*(n+1)*n + Sum_{i=1..n}
 *          (4*i*(i-1) + 1).
 * https://oeis.org/A050533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/3)*n*(5 + 6*n + 4*n^2) (OEIS A050533)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t term = N * (5 + 6 * N + 4 * N * N);
    return term / 3;
}
