/*
 * A096129  Middle term of a triple of consecutive numbers which are sums of
 *          two squares.
 * https://oeis.org/A096129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n^2 + 4*n^3 + 2*n^4
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = x * x;            /* n^2 */
    seq_t n3 = n2 * x;           /* n^3 */
    seq_t n4 = n3 * x;           /* n^4 */
    return (seq_t)1 + (seq_t)2 * n2 + (seq_t)4 * n3 + (seq_t)2 * n4;
}
