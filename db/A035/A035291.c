/*
 * A035291  Number of ways to place a non-attacking white and black queen on n
 *          X n chessboard.
 * https://oeis.org/A035291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 10*n^3 + 9*n^2 - 2*n)/3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 201, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for out-of-range or unchanged input. */
    if (n < 1)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;        /* n^2 */
    seq_t N3 = N2 * N;       /* n^3 */
    seq_t N4 = N3 * N;       /* n^4 */

    seq_t num = (seq_t)3 * N4
              - (seq_t)10 * N3
              + (seq_t)9  * N2
              - (seq_t)2  * N;

    return num / (seq_t)3;
}
