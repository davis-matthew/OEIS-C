/*
 * A006096  Gaussian binomial coefficient [n, 3] for q = 2.
 * https://oeis.org/A006096
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [n,3]_2 via q-Pascal: [m,k] = [m-1,k-1] + 2^k*[m-1,k],
 *            [m,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t g1 = 0, g2 = 0, g3 = 0;
    idx_t m;
    for (m = 1; m <= n; m++) {
        g3 = g2 + 8 * g3;
        g2 = g1 + 4 * g2;
        g1 = 1 + 2 * g1;
    }
    return g3;
}
