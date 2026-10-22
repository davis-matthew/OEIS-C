/*
 * A003417  Continued fraction for e.
 * https://oeis.org/A003417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, nice, easy
 * Formula:   a(0) = 2, a(3*n-1) = 2*n, a(m) = 1 otherwise
 * Range:     u8 n <= 382, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (sn == 0) {
        return (seq_t)2;
    }
    if (((sn + 1) % 3) == 0) {
        seq_t k = (sn + 1) / 3;
        return 2 * k;
    }
    return (seq_t)1;
}
