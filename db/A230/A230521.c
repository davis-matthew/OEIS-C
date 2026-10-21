/*
 * A230521  Number of nX3 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses dynamic programming over rows.
   Counts  n×3 arrays over {0,1,2} with the adjacency rule
   and upper left entry 0.  No closed form formula is known. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* precompute base‑3 digits for values 0..26 */
    static int digs[27][3];
    static bool init = false;
    if (!init) {
        for (int v = 0; v < 27; ++v) {
            int x = v;
            digs[v][0] = x % 3; x /= 3;
            digs[v][1] = x % 3; x /= 3;
            digs[v][2] = x;
        }
        init = true;
    }
    const int inc[3] = {1, 2, 0};   /* (x+1)%3 */

    /* state: (row_value<<3) | unsat_mask   (unsat_mask 3 bits) */
    static seq_t dp[216];
    static seq_t next[216];
    for (int i = 0; i < 216; ++i) dp[i] = 0;

    /* initialise with the first row, enforcing upper‑left = 0 */
    for (int val = 0; val < 27; ++val) {
        int *row = digs[val];
        if (row[0] != 0) continue;      /* upper left fixed to 0 */

        int mask = 0;
        for (int i2 = 0; i2 < 3; ++i2) {
            bool sat = false;
            int v = row[i2], tgt = inc[v];
            if (i2 > 0   && row[i2-1] == tgt) sat = true;
            if (i2 < 2   && row[i2+1] == tgt) sat = true;
            if (!sat) mask |= 1 << i2;
        }
        dp[(val << 3) | mask] = 1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int s = 0; s < 216; ++s)
            if (!(s & 7)) ans += dp[s];
        return ans;
    }

    /* add rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < 216; ++i) next[i] = 0;
        for (int s = 0; s < 216; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            int prev_val = s >> 3, unsat = s & 7;
            int *prev = digs[prev_val];

            for (int val = 0; val < 27; ++val) {
                int *curr = digs[val];
                /* verify that all unsatisfied cells of previous row
                   get a +1 neighbor from the new row */
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    if (!(unsat & (1 << j))) continue;
                    int vPrev = prev[j], tgt = inc[vPrev];
                    if (j > 0   && prev[j-1] == tgt) continue;
                    if (j < 2   && prev[j+1] == tgt) continue;
                    if (curr[j]   == tgt) continue;
                    if (j > 0   && curr[j-1] == tgt) continue;
                    if (j < 2   && curr[j+1] == tgt) continue;
                    ok = false;
                }
                if (!ok) continue;

                /* compute unsatisfied mask for the new row */
                int mask2 = 0;
                for (int i2 = 0; i2 < 3; ++i2) {
                    bool sat = false;
                    int v = curr[i2], tgt = inc[v];
                    if (i2 > 0   && curr[i2-1] == tgt) sat = true;
                    if (i2 < 2   && curr[i2+1] == tgt) sat = true;
                    if (prev[i2]          == tgt) sat = true;
                    if (i2 > 0   && prev[i2-1] == tgt) sat = true;
                    if (i2 < 2   && prev[i2+1] == tgt) sat = true;
                    if (!sat) mask2 |= 1 << i2;
                }
                next[(val << 3) | mask2] += ways;
            }
        }
        for (int i = 0; i < 216; ++i) dp[i] = next[i];
    }

    /* sum over final states with no unsatisfied cells */
    seq_t ans = 0;
    for (int s = 0; s < 216; ++s)
        if (!(s & 7)) ans += dp[s];
    return ans;
}
