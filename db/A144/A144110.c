/*
 * A144110  Period 6: repeat [2, 2, 2, 1, 1, 1].
 * https://oeis.org/A144110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n mod 6 < 3; a(n) = 1 otherwise
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    return (r < 3) ? (seq_t)2 : (seq_t)1;
}
