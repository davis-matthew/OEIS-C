/*
 * A010717  Period 2: repeat (5,6).
 * https://oeis.org/A010717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) for n > 1; a(0) = 5, a(1) = 6; period 2: 5,6
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is nonnegative; use modulo 2 to determine parity */
    return (seq_t)((n & 1) ? 6 : 5);
}
