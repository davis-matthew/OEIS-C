/*
 * A178617  a(n) = n^4 - (n+1)^3.
 * https://oeis.org/A178617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - (n+1)^3
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to sequence type */
    seq_t n2 = N * N;            /* n^2   */
    seq_t n4 = n2 * n2;          /* n^4   */
    seq_t N1 = N + 1;            /* n+1   */
    seq_t n13 = N1 * N1 * N1;    /* (n+1)^3 */
    return n4 - n13;
}
