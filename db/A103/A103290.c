/*
 * A103290  a(n) = n*(n-1)*(n^2-n+4)/6.
 * https://oeis.org/A103290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2-n+4)/6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* cast index to seq_t */
    seq_t term1 = N * (N - 1);          /* n*(n-1)     */
    seq_t term2 = N * N - N + 4;        /* n^2 - n + 4 */
    seq_t prod  = term1 * term2;        /* product     */
    return prod / 6;                    /* divide by 6  */
}
