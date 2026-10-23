/*
 * A158321  a(n) = 441n^2 + 2n.
 * https://oeis.org/A158321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n^2 + 2*n
 * Range:     u8 none, u16 n <= 12, u32 n <= 3120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)441 * sn * sn + (seq_t)2 * sn;
}
