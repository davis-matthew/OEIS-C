/*
 * A195024  a(n) = n*(14*n - 1).
 * https://oeis.org/A195024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^2 - n
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)14 * nn - (seq_t)1);
}
