/*
 * A213848  Principal diagonal of the convolution array A213847.
 * https://oeis.org/A213848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-4*n - 3*n^2 + 16*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 678, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1, so n>=1 */
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;          /* n^2   */
    seq_t N3 = N2 * N;         /* n^3   */
    seq_t num = (16 * N3) - (3 * N2) - (4 * N);
    return num / 3;
}
