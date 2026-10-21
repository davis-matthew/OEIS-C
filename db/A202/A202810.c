/*
 * A202810  Number of nX6 nonnegative integer arrays with each row and column
 *          increasing from zero by 0, 1 or 2.
 * https://oeis.org/A202810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts n×6 arrays with each row and column non‑decreasing and
 *            each successive difference equal to 0, 1 or 2. The first element
 *            of every row and of every column is 0. Equivalent to counting
 *            arrays of non‑negative integers whose horizontal and vertical
 *            edge increments lie in {0,1,2}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 19, u128 n <= 98,
 *            big any n
 * Helpers:   state_to_vec, vec_to_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/state_to_vec.h"
#include "../utils/vec_to_state.h"

/* Number of horizontal states: 3^5 possible 5‑digit base‑3 vectors. */
#define STATE_COUNT 243          /* 3^5 */
#define VCOUNT      243          /* 3^5 : the 5 vertical increments v[1..5] */

/* Pre‑computed vertical vectors v[0..5] with v[0] = 0, v[1..5]∈{0,1,2}. */
static unsigned char vvec[VCOUNT][6];

/* For each horizontal state s, list of reachable next states t and the
   number (0 or 1) of vertical vectors giving that transition.  */
static unsigned char trans_next[STATE_COUNT][STATE_COUNT];
static unsigned char trans_cnt[STATE_COUNT][STATE_COUNT];
static int trans_deg[STATE_COUNT];

/* Flag marking that the tables have been built. */
static bool initialized = false;

/* Build the transition tables.  Complexity: STATE_COUNT * VCOUNT * 5. */
static void init(void) {
    /* Build all vertical vectors v with v[0] = 0, v[1..5] all
       combinations of 0,1,2.  (243 vectors). */
    for (int id = 0; id < VCOUNT; ++id) {
        int tmp = id;
        vvec[id][0] = 0;           /* first column is always 0 */
        for (int k = 1; k < 6; ++k) {
            vvec[id][k] = tmp % 3;  /* v[1] .. v[5] are base‑3 digits */
            tmp /= 3;
        }
    }

    /* For every state s, compute all reachable states t. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        unsigned char hprev[5];
        state_to_vec(s, hprev);

        int deg = 0;
        for (int vid = 0; vid < VCOUNT; ++vid) {
            const unsigned char *v = vvec[vid];
            unsigned char hnew[5];
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                int val = (int)hprev[k] + (int)v[k+1] - (int)v[k];
                if (val < 0 || val > 2) { ok = false; break; }
                hnew[k] = (unsigned char)val;
            }
            if (!ok) continue;

            int t = vec_to_state(hnew);
            /* Only one vertical vector can lead to a given t,
               so the count is always 1. */
            trans_next[s][deg] = (unsigned char)t;
            trans_cnt[s][deg]  = (unsigned char)1;
            ++deg;
        }
        trans_deg[s] = deg;
    }
    initialized = true;
}

/* Main sequence function.  a(1) = 1. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined but harmless        */
    if (!initialized) init();

    /* dp[i]  = number of ways to reach horizontal state i after current row. */
    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    /* zero out dp */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;
    /* first row has all horizontal increments zero */
    const int zero_state = 0;
    dp[zero_state] = (seq_t)1;

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            int deg = trans_deg[s];
            for (int k = 0; k < deg; ++k) {
                int t = trans_next[s][k];
                unsigned char cnt = trans_cnt[s][k];   /* always 1 */
                dp_next[t] += val * (seq_t)cnt;
            }
        }
        /* copy dp_next to dp for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* Sum over all possible final horizontal states. */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        sum += dp[i];

    return sum;
}
