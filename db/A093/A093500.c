/*
 * A093500  a(n) = (15*n^2 + 5*n + 2)/2.
 * https://oeis.org/A093500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (15*n^2 + 5*n + 2)/2, n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 93, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert index to seq_t */
    return (15 * nn * nn + 5 * nn + 2) / 2;
}
