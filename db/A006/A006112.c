/*
 * A006112  Gaussian binomial coefficient [ n,3 ] for q = 5.
 * https://oeis.org/A006112
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n,3]_5 via the q-Pascal recurrence [m,k] = [m-1,k-1] +
 *            5^k*[m-1,k], with [m,0] = 1, [0,k] = 0 for k > 0; a(n) = [n,3]
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t g[4];
    idx_t m;
    g[0] = 1;
    g[1] = 0;
    g[2] = 0;
    g[3] = 0;
    for (m = 1; m <= n; m++) {
        g[3] = g[2] + g[3] * 125;
        g[2] = g[1] + g[2] * 25;
        g[1] = g[0] + g[1] * 5;
    }
    return g[3];
}
