/*
 * A070302  Number of 3 X 3 X 3 magic cubes with sum 3n.
 * https://oeis.org/A070302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^4 - 22*n^3 + 25*n^2 - 14*n + 4) / 4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 147, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;          /* Convert index to seq_t */
    seq_t n2  = N * N;              /* n^2 */
    seq_t n3  = n2 * N;             /* n^3 */
    seq_t n4  = n3 * N;             /* n^4 */

    seq_t numer = 11 * n4 - 22 * n3 + 25 * n2 - 14 * N + 4;
    return numer / 4;
}
