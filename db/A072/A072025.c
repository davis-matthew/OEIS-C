/*
 * A072025  a(n) = n^4 + 2*n^3 + 4*n^2 + 3*n + 1 = ((n+1)^5+n^5) / (2*n+1).
 * https://oeis.org/A072025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^4 + 2*n^3 + 4*n^2 + 3*n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = x * x;              /* n^2   */
    seq_t n3 = n2 * x;             /* n^3   */
    seq_t n4 = n3 * x;             /* n^4   */
    return n4 + (seq_t)2 * n3 + (seq_t)4 * n2 + (seq_t)3 * x + (seq_t)1;
}
