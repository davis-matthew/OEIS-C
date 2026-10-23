/*
 * A010698  Period 2: repeat (2,8).
 * https://oeis.org/A010698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*(-1)^n + 5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 2 for even n, 8 for odd n */
    return (seq_t)( (n & 1) ? 8U : 2U );
}
