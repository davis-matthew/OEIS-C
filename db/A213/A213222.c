/*
 * A213222  Minimum number of distinct slopes formed by n noncollinear points
 *          in the plane.
 * https://oeis.org/A213222
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/2) for n > 3; a(3) = 3
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)          /*undefined for n < 3, return 0 */
        return (seq_t)0;

    if (n == 3)
        return (seq_t)3;

    return (seq_t)(2 * (n / 2));
}
