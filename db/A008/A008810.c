/*
 * A008810  a(n) = ceiling(n^2/3).
 * https://oeis.org/A008810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ceil(n^2/3)
 * Range:     u8 n <= 27, u16 n <= 443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * nn + (seq_t)2) / (seq_t)3;
}
