/*
 * A183468  Number of nX3 binary arrays with every element equal to either the
 *          sum mod 2 of its vertical neighbors or the sum mod 2 of its
 *          horizontal neighbors.
 * https://oeis.org/A183468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183468: Number of nX3 binary arrays where each entry equals
   either the XOR of its vertical neighbors or the XOR of its horizontal
   neighbors.  Computed by dynamic programming over triples of rows. */
#include <stddef.h>

static int row_ok(int above, int row, int below) {
    for (int j = 0; j < 3; ++j) {
        int val   = (row >> j) & 1;
        int vert  = ((above >> j) & 1) ^ ((below >> j) & 1);
        int left  = (j > 0)     ? ((row >> (j-1)) & 1) : 0;
        int right = (j < 2)     ? ((row >> (j+1)) & 1) : 0;
        int horiz = left ^ right;
        if (val != vert && val != horiz) return 0;
    }
    return 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          // unused term; the sequence starts at n=1
    static const int STATES = 64;   // 8 * 8 possible (prevprev,prev) pairs
    seq_t dp[STATES], next[STATES];
    for (int i = 0; i < STATES; ++i) dp[i] = 0;

    /* Initial state: virtual row 0 above, first real row r (0..7) */
    for (int r = 0; r < 8; ++r) {
        int idx = (0 << 3) | r;   /* (0, r) */
        dp[idx] = 1;
    }

    /* Build rows 2..n */
    for (int pos = 2; pos <= n; ++pos) {
        for (int i = 0; i < STATES; ++i) next[i] = 0;
        for (int prevprev = 0; prevprev < 8; ++prevprev) {
            for (int prev = 0; prev < 8; ++prev) {
                int idx = (prevprev << 3) | prev;
                if (dp[idx] == 0) continue;
                seq_t cnt = dp[idx];
                for (int row = 0; row < 8; ++row) {
                    if (row_ok(prevprev, prev, row)) {
                        int nidx = (prev << 3) | row;
                        next[nidx] += cnt;
                    }
                }
            }
        }
        /* Copy next to dp for next iteration */
        for (int i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* Final check: last row must satisfy constraints with below=0 */
    seq_t ans = 0;
    for (int prevprev = 0; prevprev < 8; ++prevprev) {
        for (int prev = 0; prev < 8; ++prev) {
            int idx = (prevprev << 3) | prev;
            if (dp[idx] == 0) continue;
            if (row_ok(prevprev, prev, 0))
                ans += dp[idx];
        }
    }
    return ans;
}
