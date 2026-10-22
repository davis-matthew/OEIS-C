/*
 * A005911  Number of points on surface of truncated cube: a(n) = 46*n^2 + 2
 *          for n > 0.
 * https://oeis.org/A005911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 46*n^2 + 2 for n > 0
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return (n == 0) ? 1 : (46 * sn * sn + 2);
}
