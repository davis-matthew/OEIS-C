/*
 * A199264  Period 18: repeat (9,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,8).
 * https://oeis.org/A199264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = abs((n mod 18) - 9)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 18;            /* n modulo the period length */
    if (r <= 9)
        return (seq_t)(9 - r);   /* decreasing part of the wave */
    else
        return (seq_t)(r - 9);   /* increasing part of the wave */
}
