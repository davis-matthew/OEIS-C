/*
 * A105450  a(n) = binomial(n+5,6) + binomial(n+3,3) + binomial(n+2,3) +
 *          binomial(n-1,1).
 * https://oeis.org/A105450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 15*n^5 + 85*n^4 + 465*n^3 + 1354*n^2 + 2400*n) /
 *            720
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n4  = n3 * nn;          /* n^4 */
    seq_t n5  = n4 * nn;          /* n^5 */
    seq_t n6  = n5 * nn;          /* n^6 */

    seq_t num = n6
              + (seq_t)15 * n5
              + (seq_t)85 * n4
              + (seq_t)465 * n3
              + (seq_t)1354 * n2
              + (seq_t)2400 * nn;

    return num / (seq_t)720;
}
