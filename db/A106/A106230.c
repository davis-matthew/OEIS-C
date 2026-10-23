/*
 * A106230  Least k > 0 for n > 0 such that (n^2 + 1)*(k^2) + (n^2 + 1)*k + 1
 *          = j^2 where j sequence = A106229.
 * https://oeis.org/A106230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - 2*n for n > 2; a(1) = 3; a(2) = 8
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    seq_t sn = (seq_t)n;          /* convert index to seq_t first. */
    return sn * (sn - 2);          /* compute n^2 - 2*n */
}
