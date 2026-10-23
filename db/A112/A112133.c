/*
 * A112133  First differences of A112063.
 * https://oeis.org/A112133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1,11,13,11,1,23}[(n-1) mod 6]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = { 1, 11, 13, 11, 1, 23 };
    return pat[(n - 1) % 6];
}
