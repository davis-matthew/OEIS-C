/*
 * A010726  Period 2: repeat (6,10).
 * https://oeis.org/A010726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2*(-1)^n + 8 (period 2: 6,10)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even return 6, else return 10. */
    return (n & 1) ? 10 : 6;
}
