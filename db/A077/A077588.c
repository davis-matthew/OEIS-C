/*
 * A077588  Maximum number of regions into which the plane is divided by n
 *          triangles.
 * https://oeis.org/A077588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*n - 3*n + 2 for n > 0; a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;      /* n^2 */
    sq = sq * 3;             /* 3*n^2 */
    seq_t tr = nn * 3;       /* 3*n   */
    return sq - tr + 2;      /* 3*n^2 - 3*n + 2 */
}
