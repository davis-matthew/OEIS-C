/*
 * A165207  Period 4: repeat [2, 2, 4, 4].
 * https://oeis.org/A165207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + (2*floor(n/2) mod 4)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 4;
    return (r == 0 || r == 1) ? (seq_t)2 : (seq_t)4;
}
