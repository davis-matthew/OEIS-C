/*
 * A227329  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having two adjacent 1's and two
 *          adjacent 0's, with rows and columns of the latter in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A227329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 7*n^6 + 259*n^5 - 35*n^4 + 7924*n^3 * - 35252*n^2
 *            + 72456*n - 10080) / 5040 for n > 2 * a(1) = 4, a(2) = 11 * This
 *            implements the polynomial representation of A227329, * which
 *            agrees with the given recurrence for n > 10
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 78, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;

    /* use 128‑bit intermediate to avoid overflow */
    __int128 nn = n;
    __int128 n2 = nn * nn;         /* n^2   */
    __int128 n3 = n2 * nn;         /* n^3   */
    __int128 n4 = n3 * nn;         /* n^4   */
    __int128 n5 = n4 * nn;         /* n^5   */
    __int128 n6 = n5 * nn;         /* n^6   */
    __int128 n7 = n6 * nn;         /* n^7   */

    __int128 num = n7
        + (__int128)7    * n6
        + (__int128)259  * n5
        - (__int128)35   * n4
        + (__int128)7924 * n3
        - (__int128)35252* n2
        + (__int128)72456* nn
        - (__int128)10080;

    return (seq_t)(num / 5040);
}
