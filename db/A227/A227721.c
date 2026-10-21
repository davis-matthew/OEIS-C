/*
 * A227721  Floor(1/s(n)), where s(n) = (2n+1)/(2n+2) - n*log((n+1)/n).
 * https://oeis.org/A227721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*n + 9*n + 2
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return 6 * sn * sn + 9 * sn + 2;
}
