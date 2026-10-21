/*
 * A213818  Antidiagonal sums of the convolution array A213773.
 * https://oeis.org/A213818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - n^2 + 2*n) / 4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 195, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t n2  = x * x;          /* n^2 */
    seq_t n4  = n2 * n2;        /* n^4 */

    seq_t term1 = (seq_t)3 * n4;   /* 3*n^4 */
    seq_t term2 = (seq_t)2 * x;    /* 2*n */

    seq_t numerator = term1 + term2 - n2; /* 3*n^4 + 2*n - n^2 */
    return numerator / (seq_t)4;          /* divide by 4 */
}
