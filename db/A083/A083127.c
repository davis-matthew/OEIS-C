/*
 * A083127  a(n) = 3*n^3 + n^2 - 4*n.
 * https://oeis.org/A083127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 + n^2 - 4*n
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t res = 3 * x3 + x2 - 4 * x;
    return res;
}
