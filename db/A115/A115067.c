/*
 * A115067  a(n) = (3*n^2 - n - 2)/2.
 * https://oeis.org/A115067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 - n - 2)/2
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)3 * nn * nn - nn - (seq_t)2) / (seq_t)2;
}
