/*
 * A010735  Period 2: repeat (9,10).
 * https://oeis.org/A010735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 if n is even, else 10
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned: use bit test for O(1) period detection */
    return ((n & 1) == 0) ? 9 : 10;
}
