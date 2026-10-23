/*
 * A166486  Periodic sequence [0,1,1,1] of length 4; Characteristic function
 *          of numbers that are not multiples of 4.
 * https://oeis.org/A166486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n mod 4 != 0 else 0
 * Range:     u8 n <= 9495, u16 n <= 9495, u32 n <= 9495, u64 n <= 9495,
 *            u128 n <= 9495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bitwise AND to test divisibility by 4 (n % 4). */
    return (n & 3) ? 1 : 0;
}
