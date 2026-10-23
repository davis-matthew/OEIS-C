/*
 * A199394  The number of ways to color the vertices of all (11) simple
 *          unlabeled graphs on 4 nodes using at most n colors.
 * https://oeis.org/A199394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^4/3 + 4*n^3 + 10*n^2/3 + n
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2   */
    seq_t n3  = n2 * N;         /* n^3   */
    seq_t n4  = n2 * n2;        /* n^4   */

    /* Combine all terms into a single integer numerator and divide by 3 */
    seq_t num = 8 * n4 + 12 * n3 + 10 * n2 + 3 * N;

    return num / 3;
}
