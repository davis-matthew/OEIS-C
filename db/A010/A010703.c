/*
 * A010703  Period 2: repeat (3,5).
 * https://oeis.org/A010703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 - (-1)^n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) = 3 if n is even, 5 if n is odd */
    return (seq_t)((n & 1) ? 5 : 3);
}
