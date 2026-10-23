/*
 * A187452  Number of right isosceles triangles that can be formed from the
 *          n^2 points of n X n grid of points (or geoboard).
 * https://oeis.org/A187452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4-8*n^2)/12 for n even; a(n) = (5*n^4-8*n^2+3)/12
 *            for n odd
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 237, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside of offset, return 0 */
    seq_t N  = (seq_t)n;          /* convert index first */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N4 = N2 * N2;           /* n^4 */
    seq_t num = 5 * N4 - 8 * N2;   /* 5*n^4 - 8*n^2 */
    if ((n & 1) != 0)             /* if n is odd add 3 */
        num += 3;
    return num / 12;               /* integer division, exact */
}
