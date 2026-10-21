/*
 * A054620  Number of ways to color vertices of a pentagon using <= n colors,
 *          allowing only rotations.
 * https://oeis.org/A054620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 4*n) / 5
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 116, u64 n <= 9839,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 prod = tn * tn;          /* n^2   */
    prod *= tn;                                /* n^3   */
    prod *= tn;                                /* n^4   */
    prod *= tn;                                /* n^5   */
    prod += 4 * tn;                            /* n^5 + 4n */
    prod /= 5;                                 /* divide by 5 */
    return (seq_t)prod;
}
