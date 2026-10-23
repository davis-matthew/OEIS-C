/*
 * A195279  Number of lower triangles of a 3 X 3 integer array with each
 *          element differing from all of its vertical and horizontal
 *          neighbors by n or less and triangles differing by a constant
 *          counted only once.
 * https://oeis.org/A195279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (64*n^5 + 160*n^4 + 168*n^3 + 92*n^2 + 26*n + 3) / 3
 * Range:     u8 none, u16 n <= 3, u32 n <= 44, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, behaviour below offset is irrelevant */
        return 0;

    seq_t s   = (seq_t)n;     /* n */
    seq_t n2  = s * s;        /* n^2 */
    seq_t n3  = n2 * s;       /* n^3 */
    seq_t n4  = n3 * s;       /* n^4 */
    seq_t n5  = n4 * s;       /* n^5 */

    seq_t num = 64 * n5 + 160 * n4 + 168 * n3 + 92 * n2 + 26 * s + 3;
    return num / 3;
}
