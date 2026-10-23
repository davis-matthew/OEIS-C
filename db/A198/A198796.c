/*
 * A198796  a(n) = 3*6^n-1.
 * https://oeis.org/A198796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(n) = 6*a(n-1) + 5 for n>=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 2;
    for (idx_t i = 0; i < n; ++i) {
        r = r * 6 + 5;
    }
    return r;
}
