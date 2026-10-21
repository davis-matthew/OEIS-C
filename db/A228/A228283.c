/*
 * A228283  Number of nX8 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A228283: Number of n×8 binary arrays with top‑left value 1,
 * no two 1s adjacent horizontally, vertically or along the
 * northwest‑to‑southeast diagonal.
 *
 * The algorithm uses a state‑transition DP over row masks.
 * Masks are 8‑bit values with no horizontal adjacency (there are 55 of them).
 * A transition from mask i (previous row) to mask j (current row) is allowed if
 * - i and j have no vertical adjacency: (i & j) == 0
 * - and they have no NW‑SE diagonal adjacency: ((i << 1) & j) == 0
 *
 * The first row must have bit 0 set (top‑left 1).  The DP then counts
 * all possible configurations for n rows.  Complexity: O(n·55²).
 */
#include <stdbool.h>
#include <stdint.h>

/* number of masks of length 8 with no consecutive 1s */
#define STATE_CNT 55

/* store all valid 8‑bit masks and transition table */
static uint8_t masks[STATE_CNT];
static bool allowed[STATE_CNT][STATE_CNT];
static bool init = false;

static void init_tables(void)
{
    if (init) return;

    /* collect all masks with no horizontal adjacency */
    int cnt = 0;
    for (int m = 0; m < 256; ++m) {
        if ((m & (m << 1)) == 0) {     /* no adjacent 1s in the mask */
            masks[cnt++] = (uint8_t)m;
        }
    }

    /* build allowed transition table */
    for (int i = 0; i < STATE_CNT; ++i) {
        for (int j = 0; j < STATE_CNT; ++j) {
            /* vertical adjacency */
            bool ok = ((masks[i] & masks[j]) == 0);
            /* NW‑SE diagonal adjacency */
            ok = ok && (((masks[i] << 1) & masks[j]) == 0);
            allowed[i][j] = ok;
        }
    }

    init = true;
}

/*
 * Public entry point requested by the harness.
 * Returns the number of valid arrays for given n (n ≥ 1).
 */
seq_t a(idx_t n)
{
    init_tables();                       /* ensure tables are ready */

    /* dp[i] = number of arrays for the rows processed so far,
       with the last row equal to masks[i] */
    seq_t dp[STATE_CNT];
    seq_t nxt[STATE_CNT];

    /* first row: only masks with the leftmost bit set are allowed */
    for (int i = 0; i < STATE_CNT; ++i) {
        dp[i] = (masks[i] & 1) ? 1 : 0;
    }

    /* base case: only one row */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < STATE_CNT; ++i) total += dp[i];
        return total;
    }

    /* process remaining rows */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < STATE_CNT; ++i) nxt[i] = 0;

        for (int i = 0; i < STATE_CNT; ++i) {
            if (dp[i] == 0) continue;
            for (int j = 0; j < STATE_CNT; ++j) {
                if (allowed[i][j]) {
                    nxt[j] += dp[i];
                }
            }
        }

        for (int i = 0; i < STATE_CNT; ++i) dp[i] = nxt[i];
    }

    /* sum all ways for the last row */
    seq_t total = 0;
    for (int i = 0; i < STATE_CNT; ++i) total += dp[i];
    return total;
}
