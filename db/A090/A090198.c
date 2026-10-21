/*
 * A090198  a(n) = N(5,n), where N(5,x) is the 5th Narayana polynomial.
 * https://oeis.org/A090198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 10*n^3 + 20*n^2 + 10*n + 1
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 253, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */

    return n4 + (seq_t)10 * n3 + (seq_t)20 * n2 + (seq_t)10 * N + (seq_t)1;
}
