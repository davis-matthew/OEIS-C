/*
 * A003096  a(n) = a(n-1)^2 - 1, a(0) = 2.
 * https://oeis.org/A003096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)^2 - 1, a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 2;
    for (idx_t i = 0; i < n; i++) {
        result = result * result - 1;
    }
    return result;
}
