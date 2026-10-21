/*
 * A071619  a(n) = ceiling(2*n^2 / 3).
 * https://oeis.org/A071619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2*n^2/3)
 * Range:     u8 n <= 11, u16 n <= 206, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* compute 2*n^2 */
    seq_t prod = (seq_t)2 * nn * nn;

    /* ceil division by 3: (x + 2)/3 */
    return (prod + (seq_t)2) / (seq_t)3;
}
