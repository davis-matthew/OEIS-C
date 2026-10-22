/*
 * A007877  Period 4 zigzag sequence: repeat [0,1,2,1].
 * https://oeis.org/A007877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = [0,1,2,1][n % 4] (period 4 zigzag sequence)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pattern[4] = {0, 1, 2, 1};
    return pattern[n % 4];
}
