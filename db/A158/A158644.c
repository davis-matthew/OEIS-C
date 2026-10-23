/*
 * A158644  a(n) = 52*n^2 + 1.
 * https://oeis.org/A158644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 52*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 9088, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)52 * (seq_t)n * (seq_t)n + 1;
}
