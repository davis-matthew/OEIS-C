/*
 * A153449  11 times pentagonal numbers: 11*n*(3n-1)/2.
 * https://oeis.org/A153449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (33*n^2 - 11*n) / 2
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 33*n^2 - 11*n safely in seq_t */
    seq_t result = ((seq_t)33 * nn * nn - (seq_t)11 * nn) / 2;
    return result;
}
