/*
 * A178894  a(n) = n OR 7n, where OR is bitwise OR.
 * https://oeis.org/A178894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n | 7*n
 * Range:     u8 n <= 36, u16 n <= 9362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t y = x * (seq_t)7;
    return x | y;
}
