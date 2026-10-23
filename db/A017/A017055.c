/*
 * A017055  a(n) = (7*n + 6)^3.
 * https://oeis.org/A017055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = 7*x + 6;
    seq_t y = x*x;
    y = y*x;
    return y;
}
