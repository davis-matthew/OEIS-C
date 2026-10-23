/*
 * A153186  Period 9: repeat [1, 7, 4, 7, 4, 7, 1, 1, 1].
 * https://oeis.org/A153186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 9] with pattern = [1,7,4,7,4,7,1,1,1]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 9 */
    static const seq_t pattern[9] = {1, 7, 4, 7, 4, 7, 1, 1, 1};
    return pattern[n % 9];
}
