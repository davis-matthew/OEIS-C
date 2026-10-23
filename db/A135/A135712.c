/*
 * A135712  a(n) = (4*n^3 + 11*n^2 + 9*n + 2)/2.
 * https://oeis.org/A135712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 11*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (4 * nn * nn * nn + 11 * nn * nn + 9 * nn + 2) / 2;
    return val;
}
