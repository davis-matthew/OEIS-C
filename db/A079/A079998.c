/*
 * A079998  The characteristic function of the multiples of five.
 * https://oeis.org/A079998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 5 == 0, else 0
 * Range:     u8 n <= 9208, u16 n <= 9208, u32 n <= 9208, u64 n <= 9208,
 *            u128 n <= 9208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 5 == 0) ? (seq_t)1 : (seq_t)0;
}
