/*
 * A007585  10-gonal (or decagonal) pyramidal numbers: a(n) = n*(n + 1)*(8*n -
 *          5)/6.
 * https://oeis.org/A007585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(8*n-5)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 838, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return sn * (sn + 1) * (8 * sn - 5) / 6;
}
