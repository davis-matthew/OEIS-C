/*
 * A010679  Period 2: repeat (0,8).
 * https://oeis.org/A010679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n mod 2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n & 1 gives n mod 2 (0 for even, 1 for odd). */
    return (seq_t)(8 * (n & 1));
}
