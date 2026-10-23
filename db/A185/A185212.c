/*
 * A185212  a(n) = 12*n^2 - 8*n + 1.
 * https://oeis.org/A185212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n*n - 8*n + 1
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = sn * (((seq_t)12 * sn) - (seq_t)8);
    return term + 1;
}
