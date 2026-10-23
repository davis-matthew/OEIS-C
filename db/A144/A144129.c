/*
 * A144129  a(n) = ChebyshevT(3, n).
 * https://oeis.org/A144129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 3*n
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;     /* n^2 */
    seq_t cube = sq * nn;   /* n^3 */
    return 4 * cube - 3 * nn;
}
