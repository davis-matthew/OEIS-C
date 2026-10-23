/*
 * A157853  a(n) = 3600*n^2 - 1601*n + 178.
 * https://oeis.org/A157853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 - 1601*n + 178
 * Range:     u8 none, u16 n <= 4, u32 n <= 1092, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)3600 * nn * nn - (seq_t)1601 * nn + (seq_t)178;
}
