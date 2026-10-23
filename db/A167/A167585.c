/*
 * A167585  a(n) = 12*n^2 - 8*n + 9.
 * https://oeis.org/A167585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*n^2 - 8*n + 9
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)12) * nn * nn - ((seq_t)8) * nn + ((seq_t)9);
}
