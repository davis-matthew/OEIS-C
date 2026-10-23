/*
 * A158638  a(n) = 48*n^2 + 1.
 * https://oeis.org/A158638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9459, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)48 * nn * nn + 1;
}
