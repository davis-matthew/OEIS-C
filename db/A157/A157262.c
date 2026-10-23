/*
 * A157262  a(n) = 36*n^2 - 55*n + 21.
 * https://oeis.org/A157262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 55*n + 21
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 36 * nn * nn - 55 * nn + 21;
}
