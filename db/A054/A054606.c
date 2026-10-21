/*
 * A054606  a(n) = Sum_{d|7} phi(d)*n^(7/d).
 * https://oeis.org/A054606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 + 6*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */
    seq_t n7 = n6 * ns;          /* n^7 */
    return n7 + (seq_t)6 * ns;
}
