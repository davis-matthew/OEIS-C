/*
 * A165734  Period 2: repeat (6,30).
 * https://oeis.org/A165734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(3 + 2*(-1)^n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 6, a(2) = 30, repeat every 2 indices */
    return (n & 1) ? (seq_t)6 : (seq_t)30;
}
