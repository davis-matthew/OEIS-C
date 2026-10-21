/*
 * A054605  a(n) = Sum_{d|6} phi(d)*n^(6/d).
 * https://oeis.org/A054605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + n^3 + 2*n^2 + 2*n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n6 = n3 * n3;      /* n^6 */
    return n6 + n3 + (seq_t)2 * n2 + (seq_t)2 * N;
}
