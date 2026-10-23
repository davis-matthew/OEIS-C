/*
 * A194454  a(n) = 12*n^2 + 2*n + 1.
 * https://oeis.org/A194454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 + 2*n + 1
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * (seq_t)12 + sn * (seq_t)2 + (seq_t)1;
}
