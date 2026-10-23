/*
 * A131722  Period 6: repeat [0, 10, 10, 10, 10, 10].
 * https://oeis.org/A131722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 0 if n mod 6 = 0, otherwise a(n) = 10
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 6 == 0) ? (seq_t)0 : (seq_t)10;
}
