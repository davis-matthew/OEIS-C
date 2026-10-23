/*
 * A155654  a(n) = 11^n - 6^n + 1.
 * https://oeis.org/A155654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 6^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p11 = 1, p6 = 1;
    const seq_t base11 = (seq_t)11;
    const seq_t base6  = (seq_t)6;
    for (idx_t i = 0; i < n; ++i) {
        p11 *= base11;
        p6  *= base6;
    }
    return p11 - p6 + 1;
}
