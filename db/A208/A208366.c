/*
 * A208366  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208366 computed by dynamic programming
 *
 *  A208366 counts n×5 binary arrays that avoid
 *
 *      horizontal: 0 0 0   and   0 0 1
 *      vertical:   0 0 1   and   0 1 0
 *
 *  The computation proceeds by iterating over rows and keeping, for each
 *  pair of consecutive rows (previous two rows), the number of ways to
 *  obtain a valid prefix ending with that pair.  The transition rule
 *  ensures that all 2×3 sub‑matrices respect the forbidden patterns.
 *
 *  No closed form is known; the DP is O(n·states·transitions),
 *  where states = 32² = 1024 and the average number of transitions per
 *  state is small (≤ 32).  This easily satisfies the timing requirement
 *  for n ≤ 10000.
 */

#include <stdint.h>
#include <stdbool.h>

/* Constant row/row-pair sizes */
#define ROWS   32          /* 5‑bit rows, 0…31 */
#define STATES (ROWS * ROWS)

/* Pre‑computed data for DP */
static uint8_t      valid_rows[ROWS];          /* indices of valid rows */
static int          valid_count;              /* number of valid rows */
static uint16_t     trans_targets[STATES][ROWS];/* destination state indices */
static uint8_t      trans_cnt[STATES];         /* number of transitions */

/* DP temporary arrays */
static seq_t dp1[STATES];
static seq_t dp2[STATES];
static bool initialized = false;

/*-------------------------------------------------------------------------*/
/* Build the tables that encode the allowed rows and transitions.  This   */
/* is done once on the first call to a().                                 */
/*-------------------------------------------------------------------------*/
static void precompute(void)
{
    /* 1. Choose all 5–bit rows that avoid 000 and 001 horizontally. */
    valid_count = 0;
    for (int r = 0; r < ROWS; ++r) {
        bool ok = true;
        for (int c = 0; c < 3; ++c) {
            int b0 = (r >> (c + 0)) & 1;
            int b1 = (r >> (c + 1)) & 1;
            int b2 = (r >> (c + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 0 && b1 == 0 && b2 == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_rows[valid_count++] = (uint8_t)r;
        }
    }

    /* 2. Pre‑compute transitions between pairs of rows.  A transition
     *    from (a,b) to (b,t) is allowed iff for every column c the
     *    vertical 2×3 pattern (a[c], b[c], t[c]) does not match 001 or 010.
     */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int sIdx = a * ROWS + b;
            int cnt  = 0;
            for (int k = 0; k < valid_count; ++k) {
                int t = valid_rows[k];
                bool ok = true;
                for (int c = 0; c < 5; ++c) {
                    int bitA = (a >> c) & 1;
                    int bitB = (b >> c) & 1;
                    int bitT = (t >> c) & 1;
                    if ((bitA == 0 && bitB == 0 && bitT == 1) ||
                        (bitA == 0 && bitB == 1 && bitT == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    trans_targets[sIdx][cnt++] = (uint16_t)(b * ROWS + t);
                }
            }
            trans_cnt[sIdx] = (uint8_t)cnt;
        }
    }
}

/*---------------------------------------------------------------------*/
/* Main function: calculates a(n) for n ≥ 1.  For n < 1 the function
 *  returns 0 (undefined offset).                                          */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;      /* undefined, offset 1 */

    /* First two terms are trivial: valid_count and valid_count². */
    if (n == 1) return (seq_t)valid_count;
    if (n == 2) return (seq_t)valid_count * (seq_t)valid_count;

    /* DP arrays: dp_cur holds counts for the last two rows */
    seq_t *dp_cur  = dp1;
    seq_t *dp_next = dp2;

    /* Initialise dp_cur to all pairs of valid rows (first two rows). */
    for (int s = 0; s < STATES; ++s) {
        dp_cur[s]  = (seq_t)0;
        dp_next[s] = (seq_t)0;
    }
    for (int i = 0; i < valid_count; ++i) {
        int a = valid_rows[i];
        for (int j = 0; j < valid_count; ++j) {
            int b = valid_rows[j];
            dp_cur[a * ROWS + b] += (seq_t)1;
        }
    }

    /* Extend the grid row by row.  row indices are 0‑based; we already
     * have rows 0 and 1.  The loop adds rows 2 … n‑1.
     */
    for (idx_t row = 2; row < n; ++row) {
        /* reset dp_next */
        for (int s = 0; s < STATES; ++s) dp_next[s] = (seq_t)0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            uint8_t cnt = trans_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int dest = trans_targets[s][k];
                dp_next[dest] += val;
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
    }

    /* Sum all counts in dp_cur: the last two rows are irrelevant. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s) result += dp_cur[s];
    return result;
}
