/*
 * A101000  Period 3: repeat [0, 1, 3].
 * https://oeis.org/A101000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n mod 3) - 1 (periodic sequence [0, 1, 3])
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) return (seq_t)0;
    if (r == 1) return (seq_t)1;
    return (seq_t)3;
}
