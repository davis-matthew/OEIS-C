/*
 * A010700  Period 2: repeat (2,10).
 * https://oeis.org/A010700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 8*(n mod 2)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even n gives 2, odd n gives 10 */
    return (n & 1) ? (seq_t)10 : (seq_t)2;
}
