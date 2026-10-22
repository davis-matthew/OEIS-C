/*
 * A001246  Squares of Catalan numbers.
 * https://oeis.org/A001246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n)^2, where C(0) = 1 and C(k+1) = C(k)*2*(2k+1)/(k+2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k < n; k++) {
        c = c * (seq_t)(2 * (2 * k + 1));
        c = c / (seq_t)(k + 2);
    }
    return c * c;
}
