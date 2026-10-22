/*
 * A006106  Gaussian binomial coefficient [ n,3 ] for q = 4.
 * https://oeis.org/A006106
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n,3]_4 via q-Pascal rule [m,k] = [m-1,k-1] +
 *            4^k*[m-1,k], [0,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c0 = 1, c1 = 0, c2 = 0, c3 = 0;
    idx_t m;
    for (m = 1; m <= n; m++) {
        c3 = c2 + c3 * 64;
        c2 = c1 + c2 * 16;
        c1 = c0 + c1 * 4;
    }
    return c3;
}
