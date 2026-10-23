/*
 * A138129  Multiples of 1729, the Hardy-Ramanujan number.
 * https://oeis.org/A138129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1729*n
 * Range:     u8 n <= 0, u16 n <= 37, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) * 1729;
}
