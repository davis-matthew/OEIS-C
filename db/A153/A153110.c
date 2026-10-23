/*
 * A153110  Period 6: repeat [1, 5, 7, 2, 4, 8].
 * https://oeis.org/A153110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1,5,7,2,4,8}[n mod 6]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 5, 7, 2, 4, 8};
    return pattern[n % 6];
}
