/*
 * A153990  Period 6: repeat [1, 2, 5, 4, 7, 8].
 * https://oeis.org/A153990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = [1, 2, 5, 4, 7, 8] repeating every 6 terms
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 2, 5, 4, 7, 8};
    return pattern[n % 6];
}
