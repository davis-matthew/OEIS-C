/*
 * A048397  Sum of consecutive non-fourth-powers.
 * https://oeis.org/A048397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4*n^7 + 14*n^6 + 28*n^5 + 34*n^4 + 26*n^3 + 11*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 463, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */
    seq_t n6 = n5 * N;         /* n^6 */
    seq_t n7 = n6 * N;         /* n^7 */

    return 4*n7 + 14*n6 + 28*n5 + 34*n4 + 26*n3 + 11*n2 + 2*N;
}
