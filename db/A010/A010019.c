/*
 * A010019  a(0) = 1, a(n) = 29*n^2 + 2 for n>0.
 * https://oeis.org/A010019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n^2 + 2 with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sn = (seq_t)n;
    return (seq_t)29 * sn * sn + (seq_t)2;
}
