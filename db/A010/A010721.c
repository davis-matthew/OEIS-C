/*
 * A010721  Period 2: repeat (5,10).
 * https://oeis.org/A010721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 5*(n mod 2)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n mod 2 using bitwise AND, cast to seq_t, and add 5 */
    return (seq_t)(5 + 5 * (seq_t)(n & 1));
}
