/*
 * A016840  a(n) = (4*n+3)^4.
 * https://oeis.org/A016840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+3)^4
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 4 + 3;
    seq_t y = x * x;
    return y * y;
}
