/*
 * A138287  Palindromic period 10: repeat [0, 2, 8, 4, 6, 6, 4, 8, 2, 0].
 * https://oeis.org/A138287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [0, 2, 8, 4, 6, 6, 4, 8, 2, 0][n mod 10]
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[10] = {0, 2, 8, 4, 6, 6, 4, 8, 2, 0};
    return tbl[n % 10];
}
