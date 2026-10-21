/*
 * A054603  a(n) = Sum_{d|4} phi(d)*n^(4/d).
 * https://oeis.org/A054603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + n^2 + 2*n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;       /* n^2   */
    seq_t n4 = n2 * n2;       /* n^4   */
    seq_t two_n = (seq_t)2 * nn; /* 2*n  */
    return n4 + n2 + two_n;
}
