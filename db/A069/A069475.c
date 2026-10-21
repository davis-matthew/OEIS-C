/*
 * A069475  First differences of A069474, successive differences of
 *          (n+1)^6-n^6.
 * https://oeis.org/A069475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 360*n^2 + 1440*n + 1560
 * Range:     u8 none, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return 360 * sn * sn + 1440 * sn + 1560;
}
