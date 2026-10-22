/*
 * A006105  Gaussian binomial coefficient [ n,2 ] for q=4.
 * https://oeis.org/A006105
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n-1,1]_4 + 16*a(n-1), a(2) = 1, where [m,1]_4 =
 *            (4^m-1)/3 = 4*[m-1,1]_4 + 1 (q-Pascal rule for Gaussian binomial
 *            [n,2] with q=4; equals (16^n - 5*4^n + 4)/180)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;   /* [2,2]_4 */
    seq_t s = 1;   /* [1,1]_4 */
    idx_t m;
    for (m = 3; m <= n; m++) {
        s = 4 * s + 1;      /* [m-1,1]_4 */
        r = s + 16 * r;     /* [m,2]_4 */
    }
    return r;
}
