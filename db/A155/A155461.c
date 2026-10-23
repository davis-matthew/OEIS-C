/*
 * A155461  a(n) = n^2 + 52*n + 30.
 * https://oeis.org/A155461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 52*n + 30
 * Range:     u8 n <= 4, u16 n <= 231, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * x + x * (seq_t)52 + (seq_t)30;
}
