/*
 * A202415  Number of (n+2)X4 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+2)x4 binary arrays with no more than one of any
 *            consecutive * three bits set in any row or column. Computed by
 *            dynamic programming on rows
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Allowed row patterns: 4-bit masks with at most one 1 in every run of 3
     * consecutive columns.  There are 6 such patterns:
     * 0: 0000, 1: 0001, 2: 0010, 3: 0100, 4: 1000, 5: 1001. */
    static const int patt[6] = {0, 1, 2, 4, 8, 9};

    /* Precompute transition masks: nextMask[a][b] gives a 6-bit mask of all
     * patterns c such that rows a,b,c satisfy the vertical constraint
     * (any three consecutive bits in a column have at most one 1). */
    static int nextMask[6][6];
    static int initialized = 0;
    if (!initialized) {
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                int mask = 0;
                for (int c = 0; c < 6; ++c) {
                    int ok = 1;
                    for (int col = 0; col < 4; ++col) {
                        int sum = ((patt[a] >> col) & 1)
                                + ((patt[b] >> col) & 1)
                                + ((patt[c] >> col) & 1);
                        if (sum > 1) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok)
                        mask |= 1 << c;
                }
                nextMask[a][b] = mask;
            }
        }
        initialized = 1;
    }

    idx_t N = n + 2;               /* number of rows in the array */
    /* State is a pair of consecutive rows (prev_prev, prev).  There are 6*6=36 states. */
    seq_t dp[36]      = {0};
    seq_t ndp[36]     = {0};

    /* Initialise DP after the first two rows (no vertical constraint yet). */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            dp[a * 6 + b] = 1;

    /* Process rows 3..N.  Each iteration adds one more row and updates states. */
    for (idx_t r = 2; r < N; ++r) {
        for (int i = 0; i < 36; ++i)
            ndp[i] = 0;

        for (int s = 0; s < 36; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            int a = s / 6;
            int b = s % 6;
            int mask = nextMask[a][b];
            for (int c = 0; c < 6; ++c) {
                if (mask & (1 << c)) {
                    ndp[b * 6 + c] += cur;
                }
            }
        }
        /* copy ndp to dp for the next iteration */
        for (int i = 0; i < 36; ++i)
            dp[i] = ndp[i];
    }

    /* Sum all states to obtain the total number of valid arrays. */
    seq_t result = 0;
    for (int i = 0; i < 36; ++i)
        result += dp[i];
    return result;
}
