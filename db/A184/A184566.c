/*
 * A184566  Number of (n+2)X3 0..3 arrays with each 3X3 subblock having rows
 *          and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over rows. There is no
 *            known closed form; the routine counts all (n+2)×3 arrays with
 *            entries 0..3 such that every 3×3 subblock has lexicographically
 *            nondecreasing rows and columns
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 59, u128 n <= 200,
 *            big any n
 * Helpers:   lex_le3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/lex_le3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range */

    /*------------------------------------------------
     *  Pre‑compute state data (executed only once)
     *-----------------------------------------------*/
    static bool initialized = false;

    /* 3‑tuples for the 64 possible rows 0..3×3-1 (base‑4) */
    static int row_val[64][3];

    /* mapping from (prev,curr) to state id; -1 for invalid */
    static int pair_id[64][64];

    /* state lists for transitions */
    static int state_prev[2080];
    static int state_curr[2080];
    static int next_len[2080];
    static int next_row[2080][64];

    /* number of valid states (prev <= curr) */
    static int state_count = 0;

    if (!initialized) {
        /* encode the rows */
        for (int i = 0; i < 64; ++i) {
            row_val[i][0] = i >> 4;          /* high 2 bits   */
            row_val[i][1] = (i >> 2) & 3;    /* middle 2 bits */
            row_val[i][2] = i & 3;           /* low 2 bits    */
            for (int j = 0; j < 64; ++j)
                pair_id[i][j] = -1;
        }

        /* enumerate all ordered pairs (prev) <= (curr) */
        for (int p = 0; p < 64; ++p) {
            for (int c = p; c < 64; ++c) {
                pair_id[p][c] = state_count;
                state_prev[state_count] = p;
                state_curr[state_count] = c;
                ++state_count;                /* never exceeds 2080 */
            }
        }

        /* pre‑compute transitions */
        for (int sid = 0; sid < state_count; ++sid) {
            int p = state_prev[sid];
            int q = state_curr[sid];
            int len = 0;
            for (int r = q; r < 64; ++r) {    /* next row must satisfy q <= r */
                int a0 = row_val[p][0], a1 = row_val[q][0], a2 = row_val[r][0];
                int b0 = row_val[p][1], b1 = row_val[q][1], b2 = row_val[r][1];
                int c0 = row_val[p][2], c1 = row_val[q][2], c2 = row_val[r][2];

                if (!lex_le3(a0, a1, a2, b0, b1, b2)) continue;
                if (!lex_le3(b0, b1, b2, c0, c1, c2)) continue;

                next_row[sid][len++] = r;
            }
            next_len[sid] = len;
        }
        initialized = true;
    }

    /*------------------------------------------------
     *  Dynamic programming over the rows of the array
     *  Array size = (n+2) × 3, rows numbered 1..n+2
     *-----------------------------------------------*/
    const idx_t H = n + 2;               /* total number of rows */

    /* DP arrays for the current and next step */
    static seq_t dp0[2080], dp1[2080];
    seq_t *dp_cur   = dp0;
    seq_t *dp_next  = dp1;

    const seq_t one = (seq_t)1;

    /* initialise: all states are possible after the first two rows */
    for (int sid = 0; sid < state_count; ++sid)
        dp_cur[sid] = one;

    /* add rows 3 .. H */
    for (idx_t row = 3; row <= H; ++row) {
        /* zero next DP array */
        for (int sid = 0; sid < state_count; ++sid)
            dp_next[sid] = (seq_t)0;

        /* transition from each current state */
        for (int sid = 0; sid < state_count; ++sid) {
            seq_t val = dp_cur[sid];
            if (val == (seq_t)0) continue;
            int q = state_curr[sid];
            int len = next_len[sid];
            for (int k = 0; k < len; ++k) {
                int r = next_row[sid][k];
                int next_sid = pair_id[q][r];   /* guaranteed valid */
                dp_next[next_sid] += val;
            }
        }
        /* swap for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states */
    seq_t ans = (seq_t)0;
    for (int sid = 0; sid < state_count; ++sid)
        ans += dp_cur[sid];

    return ans;
}
