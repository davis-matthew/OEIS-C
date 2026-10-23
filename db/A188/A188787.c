/*
 * A188787  Number of 4-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 392*n^4 - 1128*n^3 + 912*n^2 - 192*n for n > 2; a(1) = 0,
 *            a(2) = 576
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 576;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t term1 = (seq_t)392 * n4;  /* 392 * n^4 */
    seq_t term2 = (seq_t)1128 * n3; /* 1128 * n^3 */
    seq_t term3 = (seq_t)912  * n2; /* 912  * n^2 */
    seq_t term4 = (seq_t)192  * nn; /* 192  * n   */

    return term1 - term2 + term3 - term4;
}
