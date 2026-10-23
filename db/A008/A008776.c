/*
 * A008776  Pisot sequences E(2,6), L(2,6), P(2,6), T(2,6).
 * https://oeis.org/A008776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)2;
    for (idx_t i = 0; i < n; ++i) {
        val *= (seq_t)3;
    }
    return val;
}
