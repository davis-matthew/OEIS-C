/*
 * A131973  Period 8: repeat 121, 242, 363, 484, 605, 726, 847, 968.
 * https://oeis.org/A131973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121 * ((n mod 8) + 1)
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 8;           /* n modulo 8 */
    return (seq_t)121 * ((seq_t)r + (seq_t)1);
}
