/*
 * A007051  a(n) = (3^n + 1)/2.
 * https://oeis.org/A007051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   a(n) = 3*a(n-1) - 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        v = v * 3 - 1;
    }
    return v;
}
