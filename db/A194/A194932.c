/*
 * A194932  Number of lower triangles of a 3 X 3 0..n array with no element
 *          differing from any of its horizontal or vertical neighbors by more
 *          than one.
 * https://oeis.org/A194932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 171*n - 127 for n > 2 a(1) = 64, a(2) = 217
 * Range:     u8 n <= 2, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 64;
    if (n == 2) return 217;
    return (seq_t)171 * (seq_t)n - 127;
}
