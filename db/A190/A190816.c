/*
 * A190816  a(n) = 5*n^2 - 4*n + 1.
 * https://oeis.org/A190816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 - 4*n + 1
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 5 * nn * nn - 4 * nn + 1;
}
