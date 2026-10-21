/*
 * A228664  Number of 5 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228664 – number of 5×n binary arrays with top‑left 1 and no two 1s adjacent horizontally or on either diagonal.
   Computation method: dynamic programming over column masks.
   For each column 0..n-1 we keep a 5‑bit mask of which cells in that column contain a 1.
   Vertical adjacency is allowed, so the 5 bits are free except for the top‑left cell in the first column,
   which must be 1.  Two consecutive columns (m, n) are compatible iff
        (m & n) == 0   &&   ((m<<1) & n) == 0   &&   ((m>>1) & n) == 0
   (no same row, no (row±1) diagonal neighbor).  The DP evolves as
        dp[i][m] = Σ_{prev} dp[i‑1][prev]   over all prev compatible with m.
   The answer is Σ_m dp[n‑1][m] (for n≥1).  For n=1 the only restriction is top‑left 1,
   giving 2⁴=16 possibilities.  The implementation below supports arbitrary‑precision
   seq_t and is fully O(32²·n) in time and O(32) in space.  The sequence satisfies
   the empirical recurrence given in OEIS, but we compute it directly. */

#include <stdbool.h>

#define STATE_COUNT 32   /* 5 bits → 2⁵ possibilities */

static bool compat[STATE_COUNT][STATE_COUNT];
static bool compat_init_done = false;

static void init_compat(void)
{
    if (compat_init_done) return;
    for (int m = 0; m < STATE_COUNT; ++m) {
        for (int n = 0; n < STATE_COUNT; ++n) {
            compat[m][n] = ((m & n) == 0 &&
                            ((m << 1) & n) == 0 &&
                            ((m >> 1) & n) == 0);
        }
    }
    compat_init_done = true;
}

/* Return the number of 5×n binary arrays with the top‑left value 1 and
   forbidden horizontal or diagonal adjacency.  n is 1‑based (a(1)=16). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset, safe to return 0 */

    init_compat();

    seq_t dp_prev[STATE_COUNT];
    seq_t dp_curr[STATE_COUNT];

    /* initialise dp for column 0: must have bit 0 set */
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;
    for (int m = 0; m < STATE_COUNT; ++m)
        if (m & 1) dp_prev[m] = 1;

    /* DP over subsequent columns */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_curr[i] = 0;
        for (int curr = 0; curr < STATE_COUNT; ++curr) {
            seq_t sum = 0;
            for (int prev = 0; prev < STATE_COUNT; ++prev)
                if (compat[prev][curr])
                    sum += dp_prev[prev];
            dp_curr[curr] = sum;
        }
        /* swap dp_prev and dp_curr */
        for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = dp_curr[i];
    }

    /* sum all possibilities for the last column */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp_prev[i];
    return ans;
}
