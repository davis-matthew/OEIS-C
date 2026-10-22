/*
 * A002620  Quarter-squares: a(n) = floor(n/2)*ceiling(n/2). Equivalently,
 *          a(n) = floor(n^2/4).
 * https://oeis.org/A002620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = floor(n/2) * ceiling(n/2)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t q = sn >> 1;
    seq_t r = (sn + 1) >> 1;
    return q * r;
}
