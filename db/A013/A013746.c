/*
 * A013746  a(n) = 10^(3*n + 1).
 * https://oeis.org/A013746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 10;          /* 10^(1) */
    for (idx_t i = 0; i < n; ++i) {
        result = result * 1000; /* multiply by 10^3 each step */
    }
    return result;
}
