/*
 * A002812  a(n) = 2*a(n-1)^2 - 1, starting a(0) = 2.
 * https://oeis.org/A002812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1)^2 - 1, a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)2;
    for (idx_t i = 1; i <= n; i++) {
        result = 2 * result * result - 1;
    }
    return result;
}
