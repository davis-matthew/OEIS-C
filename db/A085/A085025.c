/*
 * A085025  a(n) = (5*n+1)*(5*n+6).
 * https://oeis.org/A085025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 1) * (5*n + 6) (Offset: 0)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t left  = 5 * nn + 1;
    seq_t right = 5 * nn + 6;
    return left * right;
}
