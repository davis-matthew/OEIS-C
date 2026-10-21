/*
 * A050408  a(n) = (117*n^2 - 99*n + 2)/2.
 * https://oeis.org/A050408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (117*n^2 - 99*n + 2)/2
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 6124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                     /* convert index to seq_t */
    seq_t term = (117 * nn * nn - 99 * nn + 2) / 2;
    return term;
}
