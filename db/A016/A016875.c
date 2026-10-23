/*
 * A016875  a(n) = (5n+2)^3.
 * https://oeis.org/A016875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+2)^3
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = 5 * x + 2;
    return y * y * y;
}
