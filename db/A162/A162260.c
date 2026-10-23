/*
 * A162260  a(n) = (n^3 + 4*n^2 - n)/2.
 * https://oeis.org/A162260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 4*n^2 - n)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn * sn + 4 * sn * sn - sn;
    return res / 2;
}
