/*
 * A156843  a(n) = 279841n^2 - 165048n + 24335.
 * https://oeis.org/A156843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 279841*n*n - 165048*n + 24335
 * Range:     u8 none, u16 n <= 0, u32 n <= 124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)279841 * nn * nn - (seq_t)165048 * nn + (seq_t)24335;
}
