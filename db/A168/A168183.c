/*
 * A168183  Numbers that are not multiples of 9.
 * https://oeis.org/A168183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-1)/8)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n + (seq_t)((n - 1) / 8);
}
