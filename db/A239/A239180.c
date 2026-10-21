/*
 * A239180  Number of (2+1)X(n+1) 0..2 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A239180: Number of 3×(n+1) 0..2 arrays with no element greater than all horizontal neighbors
   or equal to all vertical neighbors.  Computed with a transfer‑matrix DP over the 12
   admissible column vectors.  Offset 1.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, return 0 for safety */

    /* All mutually admissible column vectors (top,mid,bottom) that satisfy the
       vertical condition (mid ≠ top and mid ≠ bottom). */
    static const int vec[12][3] = {
        {0,1,0}, {0,2,0}, {0,2,1}, {0,1,2},
        {1,2,0}, {1,0,1}, {1,2,1}, {1,0,2},
        {2,1,0}, {2,0,1}, {2,0,2}, {2,1,2}
    };
    const int V = 12;          /* number of column states */
    const int MASK = 8;        /* 3‑bit flags, one per row */

    seq_t dp[V][MASK];
    seq_t nxt[V][MASK];

    /* initialise first column: all rows are considered 'greater than left neighbor' */
    for (int i = 0; i < V; ++i)
        for (int m = 0; m < MASK; ++m)
            dp[i][m] = 0;
    for (int i = 0; i < V; ++i) dp[i][7] = (seq_t)1;   /* all three bits set */

    /* Process each of the n remaining columns */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero nxt */
        for (int i = 0; i < V; ++i)
            for (int m = 0; m < MASK; ++m)
                nxt[i][m] = 0;

        for (int i = 0; i < V; ++i) {
            for (int m = 0; m < MASK; ++m) {
                seq_t cnt = dp[i][m];
                if (cnt == 0) continue;

                /* try every possible next column vector */
                for (int j = 0; j < V; ++j) {
                    int newMask = 0;
                    int ok = 1;

                    for (int r = 0; r < 3; ++r) {
                        int vi = vec[i][r];
                        int vj = vec[j][r];

                        if ((m >> r) & 1) {            /* previous row > its left neighbor */
                            if (vi > vj) {            /* would be > both neighbors -> illegal */
                                ok = 0; break;
                            }
                        }
                        if (vj > vi) newMask |= 1 << r; /* will be > next neighbor */
                    }
                    if (!ok) continue;
                    nxt[j][newMask] += cnt;
                }
            }
        }

        /* copy nxt back to dp for next iteration */
        for (int i = 0; i < V; ++i)
            for (int m = 0; m < MASK; ++m)
                dp[i][m] = nxt[i][m];
    }

    /* Sum all final states where the last column is not greater than its left neighbor */
    seq_t result = 0;
    for (int i = 0; i < V; ++i)
        result += dp[i][0];

    return result;
}
