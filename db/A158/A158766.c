/*
 * A158766  a(n) = 1444*n^2 + 38.
 * https://oeis.org/A158766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1444*n^2 + 38
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 1724, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * (seq_t)1444 + (seq_t)38;
}
