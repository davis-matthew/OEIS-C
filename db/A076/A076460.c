/*
 * A076460  Sum of squares of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly one way.
 * https://oeis.org/A076460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(7*n^4+2*n^3-6*n^2-n+1)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn   = (seq_t)n;          /* cast index to sequence type */
    seq_t n2   = sn * sn;           /* n^2 */
    seq_t n3   = n2 * sn;           /* n^3 */
    seq_t n4   = n2 * n2;           /* n^4 */
    seq_t poly = (seq_t)7 * n4 + (seq_t)2 * n3 - (seq_t)6 * n2 - sn + (seq_t)1;
    seq_t nnp1 = sn * (sn + 1);     /* n*(n+1) */
    return (nnp1 * poly) / (seq_t)6;
}
