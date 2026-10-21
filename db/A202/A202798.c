/*
 * A202798  Number of nX4 binary arrays with every one adjacent to another one
 *          horizontally or vertically.
 * https://oeis.org/A202798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 binary arrays with every 1 adjacent to another 1
 *            horizontally or vertically
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   compute_need
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/compute_need.h"

/* DFA over rows: 81 states. */
#define MAX_PAT 16
#define MAX_STATES 81

static int state_id[MAX_PAT][MAX_PAT];
static int pattern_of[MAX_STATES];
static int need_of[MAX_STATES];
static bool need_zero[MAX_STATES];
static int transitions[MAX_STATES][MAX_PAT];
static int trans_cnt[MAX_STATES];
static int nstates;

/* Initialise state tables. */
static void init_states(void)
{
    for (int i = 0; i < MAX_PAT; ++i)
        for (int j = 0; j < MAX_PAT; ++j)
            state_id[i][j] = -1;

    nstates = 0;
    for (int p = 0; p < MAX_PAT; ++p) {
        for (int k = 0; k < MAX_PAT; ++k) {
            if ((k & ~p) != 0) continue;       /* k must be a subset of p */
            pattern_of[nstates] = p;
            need_of[nstates] = k;
            need_zero[nstates] = (k == 0);
            state_id[p][k] = nstates;
            ++nstates;
        }
    }

    for (int s = 0; s < nstates; ++s) {
        int p = pattern_of[s];
        int k = need_of[s];
        int cnt = 0;
        for (int q = 0; q < MAX_PAT; ++q) {
            if ((k & ~q) != 0) continue;         /* all needed bits must be present in q */
            int nk = compute_need(q, p);
            int t = state_id[q][nk];
            if (t >= 0) {
                transitions[s][cnt++] = t;
            }
        }
        trans_cnt[s] = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */

    static bool initialized = false;
    if (!initialized) {
        init_states();
        initialized = true;
    }

    static seq_t dp[MAX_STATES];
    static seq_t dp_next[MAX_STATES];

    for (int i = 0; i < nstates; ++i) dp[i] = 0;

    for (int p = 0; p < MAX_PAT; ++p) {
        int need = compute_need(p, 0);
        int sid = state_id[p][need];
        if (sid >= 0) {
            dp[sid] = dp[sid] + (seq_t)1;
        }
    }

    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < nstates; ++i) dp_next[i] = 0;
        for (int s = 0; s < nstates; ++s) {
            seq_t cur = dp[s];
            if (cur == (seq_t)0) continue;
            int cnt = trans_cnt[s];
            for (int t_idx = 0; t_idx < cnt; ++t_idx) {
                int t = transitions[s][t_idx];
                dp_next[t] = dp_next[t] + cur;
            }
        }
        for (int i = 0; i < nstates; ++i) dp[i] = dp_next[i];
    }

    seq_t result = 0;
    for (int s = 0; s < nstates; ++s) {
        if (need_zero[s]) result = result + dp[s];
    }

    return result;
}
