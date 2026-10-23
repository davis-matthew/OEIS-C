/*
 * A181640  Partial sums of floor(n^2/5) (A118015).
 * https://oeis.org/A181640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 11*n + 6)/30)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type early */
    seq_t N = (seq_t)n;

    /* compute N^2 and N^3 */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    /* numerator = 2*N^3 + 3*N^2 - 11*N + 6 */
    seq_t numerator = (seq_t)2 * N3 + (seq_t)3 * N2 - (seq_t)11 * N + (seq_t)6;

    /* divide by 30, truncating toward zero for unsigned types */
    return numerator / (seq_t)30;
}
