/*
 * A158768  a(n) = 1521*n^2 + 39.
 * https://oeis.org/A158768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1521*n^2 + 39
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 1680, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 1521 * nn * nn + 39;
}
