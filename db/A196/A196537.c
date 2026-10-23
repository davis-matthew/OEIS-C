/*
 * A196537  Number of nX1 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors less than or equal to
 *          itself.
 * https://oeis.org/A196537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=2, a(4)=7; for n >= 5, a(n) = a(n-1) +
 *            a(n-2) + 3*a(n-3) + a(n-4) (the sequence satisfies this
 *            recurrence)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;                     // base case

    seq_t dp[3][3] = {0}, ndp[3][3] = {0};

    /* initialize DP after placing the first two elements */
    for (int a = 0; a < 3; ++a) {
        for (int b = 0; b < 3; ++b) {
            int condition = (b <= a);                 /* 1 if b <= a, else 0 */
            if (a == condition)
                dp[a][b] = 1;
        }
    }

    /* process positions 3..n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        /* zero ndp */
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                ndp[i][j] = 0;

        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                seq_t cur = dp[x][y];
                if (cur == 0) continue;
                for (int z = 0; z < 3; ++z) {
                    int cnt = (x <= y) + (z <= y);    /* number of neighbors <= y */
                    if (y == cnt) {
                        ndp[y][z] += cur;
                    }
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* finalize: check condition for the last element */
    seq_t result = 0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            seq_t cnt = dp[x][y];
            if (cnt == 0) continue;
            int cond = (x <= y);
            if (y == cond)
                result += cnt;
        }
    }
    return result;
}
