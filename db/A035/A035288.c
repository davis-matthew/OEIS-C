/*
 * A035288  Number of ways to place a non-attacking white and black bishop on
 *          n X n chessboard.
 * https://oeis.org/A035288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 4*n^3 + 3*n^2 - 2*n)/3
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t n4 = n3 * nn;      /* n^4 */

    /* Compute numerator as (3*n^4 + 3*n^2) - (4*n^3 + 2*n)
     * to avoid unsigned underflow. */
    seq_t part1 = 3 * n4 + 3 * n2;    /* 3*n^4 + 3*n^2 */
    seq_t part2 = 4 * n3 + 2 * nn;    /* 4*n^3 + 2*n  */
    seq_t numer = part1 - part2;      /* non‑negative for all n>=1 */

    return numer / 3;
}
