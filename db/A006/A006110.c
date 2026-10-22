/*
 * A006110  Gaussian binomial coefficient [n, 5] for q = 2.
 * https://oeis.org/A006110
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n,5]_2, computed by the q-Pascal recurrence [m,k] =
 *            [m-1,k-1] + 2^k*[m-1,k], with [0,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t g[6];
    int k;
    idx_t m;
    g[0] = 1;
    for (k = 1; k <= 5; k++) g[k] = 0;
    for (m = 1; m <= n; m++) {
        for (k = 5; k >= 1; k--) {
            g[k] = g[k - 1] + g[k] * (1 << k);
        }
    }
    return g[5];
}
