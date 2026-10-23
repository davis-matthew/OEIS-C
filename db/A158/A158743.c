/*
 * A158743  a(n) = 1369*n^2 - 37.
 * https://oeis.org/A158743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1369*n^2 - 37
 * Range:     u8 none, u16 n <= 6, u32 n <= 1771, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = nn * nn;
    result *= (seq_t)1369;
    result -= (seq_t)37;
    return result;
}
