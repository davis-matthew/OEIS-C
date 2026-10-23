/*
 * A168122  a(n) = n^2*(n^4+1)/2.
 * https://oeis.org/A168122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^4+1)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2  */
    seq_t n4 = n2 * n2;        /* n^4  */
    seq_t tmp = n4 + (seq_t)1; /* n^4 + 1 */
    seq_t prod = n2 * tmp;     /* n^2 * (n^4+1) */
    return prod / ((seq_t)2);
}
