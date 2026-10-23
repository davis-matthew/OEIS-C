/*
 * A158741  a(n) = 1369*n^2 + 37.
 * https://oeis.org/A158741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1369*n^2 + 37
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 1771, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)1369 * nn * nn + (seq_t)37;
}
