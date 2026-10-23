/*
 * A100242  a(n) = n^5 - n^2*(n^2 - 1)/2.
 * https://oeis.org/A100242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - n^2*(n^2 - 1)/2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                   /* n as seq_t */
    seq_t n2 = nn * nn;                   /* n^2          */
    seq_t n4 = n2 * n2;                   /* n^4          */
    seq_t term1 = n4 * nn;                /* n^5          */
    seq_t term2 = (n2 * (n2 - (seq_t)1)) / (seq_t)2;  /* n^2*(n^2-1)/2 */
    return term1 - term2;
}
