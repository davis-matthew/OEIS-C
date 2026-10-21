/*
 * A061224  a(n) = n^2 + (n + 1)^3 + (n + 2)^4 + (n + 3)^5.
 * https://oeis.org/A061224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + (n + 1)^3 + (n + 2)^4 + (n + 3)^5
 * Range:     u8 none, u16 n <= 6, u32 n <= 81, u64 n <= 7128, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n^5 + 16*n^4 + 99*n^3 + 298*n^2 + 440*n + 260   (expanded) */

seq_t a(idx_t n)
{
    /* convert index to seq_t once for reuse */
    seq_t N = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = N * N;          /* n^2   */
    seq_t n3 = n2 * N;         /* n^3   */
    seq_t n4 = n3 * N;         /* n^4   */
    seq_t n5 = n4 * N;         /* n^5   */

    /* compute the polynomial sum */
    return n5 + (seq_t)16 * n4 + (seq_t)99 * n3 + (seq_t)298 * n2 + (seq_t)440 * N + (seq_t)260;
}
