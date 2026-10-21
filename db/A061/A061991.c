/*
 * A061991  Number of ways to place 5 nonattacking queens on a 5 X n board.
 * https://oeis.org/A061991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - 30*n^4 + 407*n^3 - 3098*n^2 + 13104*n - 24332, n >=
 *            11 * For n < 11 the values are:
 *            0,0,0,0,0,10,40,164,568,1614,3916
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n = 0 .. 10 */
    static const seq_t base[11] = {
        0, 0, 0, 0, 0, 10, 40, 164, 568, 1614, 3916
    };

    if (n < 11) {
        return base[n];
    }

    /* use the explicit polynomial for n >= 11 */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t n4 = n3 * ns;          /* n^4 */
    seq_t n5 = n4 * ns;          /* n^5 */

    seq_t res = n5;
    res -= (seq_t)30 * n4;
    res += (seq_t)407 * n3;
    res -= (seq_t)3098 * n2;
    res += (seq_t)13104 * ns;
    res -= (seq_t)24332;

    return res;
}
