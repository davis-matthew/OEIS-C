/*
 * A003464  a(n) = (6^n - 1)/5.
 * https://oeis.org/A003464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 0; i < n; i++) {
        result = result * 6 + 1;
    }
    return result;
}
