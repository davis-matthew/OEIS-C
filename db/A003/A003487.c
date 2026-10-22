/*
 * A003487  a(n) = a(n-1)^2 - 2, with a(0) = 5.
 * https://oeis.org/A003487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^2 - 2, with a(0) = 5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)5;
    for (idx_t i = 1; i <= n; i++) {
        val = val * val - (seq_t)2;
    }
    return val;
}
