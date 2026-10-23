/*
 * A010674  Period 2: repeat (0,3).
 * https://oeis.org/A010674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 3*(n mod 2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 2 is 0 or 1; multiply by 3 to get 0 or 3 */
    return (seq_t)((n & 1) * 3);
}
