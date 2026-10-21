/*
 * A090197  a(n) = n^3 + 6*n^2 + 6*n + 1.
 * https://oeis.org/A090197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 6*n^2 + 6*n + 1
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;           /* cast index to sequence type */
    seq_t n_sq = N * N;           /* n^2 */
    seq_t result = N * n_sq;      /* n^3 */
    result += (seq_t)6 * n_sq;    /* + 6*n^2 */
    result += (seq_t)6 * N;       /* + 6*n */
    result += 1;                  /* + 1 */
    return result;
}
