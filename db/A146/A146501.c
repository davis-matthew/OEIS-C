/*
 * A146501  Period 6: repeat [4, 8, 7, 5, 1, 2].
 * https://oeis.org/A146501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = [4, 8, 7, 5, 1, 2][n % 6]
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pattern[6] = {4, 8, 7, 5, 1, 2};
    return pattern[(unsigned)(n % 6)];
}
