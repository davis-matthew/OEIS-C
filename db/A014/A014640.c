/*
 * A014640  Even heptagonal numbers (A000566).
 * https://oeis.org/A014640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^2-3*n for even n; a(n) = 10*n^2+7*n+1 for odd n
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    if ((n & 1) == 0) {
        return (seq_t)10 * n2 - (seq_t)3 * sn;
    } else {
        return (seq_t)10 * n2 + (seq_t)7 * sn + (seq_t)1;
    }
}
