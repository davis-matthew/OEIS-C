/*
 * A071229  a(n) = n*(14*n^2 - 21*n + 13)/6.
 * https://oeis.org/A071229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(14*n^2 - 21*n + 13)/6, offset 0
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 717, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* careful cast to avoid overflow of idx_t */
    seq_t n2 = N * N;             /* square of n   */
    seq_t temp = 14 * n2 - 21 * N + 13;  /* inner polynomial */
    return N * temp / 6;          /* final division by 6 */
}
