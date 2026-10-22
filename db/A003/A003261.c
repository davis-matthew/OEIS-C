/*
 * A003261  Woodall (or Riesel) numbers: n*2^n - 1.
 * https://oeis.org/A003261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*2^n - 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = 1;
    for (idx_t i = 0; i < n; i++) {
        power = power + power;
    }
    return (seq_t)n * power - 1;
}
