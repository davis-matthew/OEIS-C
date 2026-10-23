/*
 * A010705  Period 2: repeat (3,7).
 * https://oeis.org/A010705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n%2) + 3 (i.e., 3 if n is even, 7 if n is odd)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bitwise test for parity: n & 1 is 0 for even, 1 for odd */
    return (n & 1) ? 7 : 3;
}
