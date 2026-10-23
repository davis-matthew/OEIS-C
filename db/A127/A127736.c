/*
 * A127736  a(n) = n*(n^2 + 2*n - 1)/2.
 * https://oeis.org/A127736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 2*n - 1)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = n;                /* convert index to seq_t */
    seq_t n_sq = nn * nn;          /* n^2 */
    seq_t t    = n_sq + nn + nn - 1;/* n^2 + 2*n - 1 */
    return (nn * t) / 2;           /* (n * (n^2 + 2*n - 1)) / 2 */
}
