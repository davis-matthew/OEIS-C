/*
 * A237369  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median plus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   absdiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by dynamic programming on 3‑column
 * 0..2 arrays of size (n+1)×3.  For each 2×2 subblock the sum of its
 * four entries must differ by exactly one from the sum of its horizontal
 * or vertical neighbour.  The algorithm counts all such arrays for
 * any n ≥ 1.  The computation is O(n·S) where S ≈ 2 k states.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/absdiff.h"

#define ROWS   3            /* number of columns */
#define V      3            /* possible values 0,1,2 */
#define STATE_ROWS(V, ROWS) ((V)*(V)*(V))  /* 27 */

#define SUMSIZE 9           /* sums 0..8 */
#define S1CNT SUMSIZE
#define S2CNT SUMSIZE
#define STATE_S 81          /* 9×9 */
#define STATES (STATE_ROWS(V, ROWS) * STATE_S) /* 27×81 = 2187 */
#define MAX_TRANS_PER_STATE 27                  /* at most 27 */

typedef int16_t s16;          /* for transition indices */

/* precomputed arrays */
static int8_t pattern[STATES/STATE_S][ROWS];            /* 27 × 3 */
static uint8_t sum1[STATE_ROWS(V, ROWS)][STATE_ROWS(V, ROWS)];
static uint8_t sum2[STATE_ROWS(V, ROWS)][STATE_ROWS(V, ROWS)];
static uint8_t horiz_ok[STATE_ROWS(V, ROWS)][STATE_ROWS(V, ROWS)];
static s16 trans[STATES][MAX_TRANS_PER_STATE];
static uint8_t trans_cnt[STATES];
static int initialized = 0;

/* helper: |a-b| */

/* init all the precomputed tables once */
static void init_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* map 0..26 to 3‑digit base‑3 patterns */
    for (int id = 0; id < STATE_ROWS(V, ROWS); ++id) {
        int x = id;
        for (int c = 2; c >= 0; --c) {
            pattern[id][c] = x % V;
            x /= V;
        }
    }

    /* compute pair sums */
    for (int p = 0; p < STATE_ROWS(V, ROWS); ++p) {
        for (int q = 0; q < STATE_ROWS(V, ROWS); ++q) {
            uint8_t s1 = pattern[p][0] + pattern[p][1]
                       + pattern[q][0] + pattern[q][1];
            uint8_t s2 = pattern[p][1] + pattern[p][2]
                       + pattern[q][1] + pattern[q][2];
            sum1[p][q] = s1;
            sum2[p][q] = s2;
            horiz_ok[p][q] = absdiff(s1, s2) == 1;
        }
    }

    /* precompute transitions */
    for (int state = 0; state < STATES; ++state) {
        int cum = state / STATE_S;
        int s1prev = (state % STATE_S) / SUMSIZE;
        int s2prev = (state % STATE_S) % SUMSIZE;
        trans_cnt[state] = 0;
        for (int nb = 0; nb < STATE_ROWS(V, ROWS); ++nb) {
            uint8_t s1 = sum1[cum][nb];
            uint8_t s2 = sum2[cum][nb];
            if (!horiz_ok[cum][nb]) continue;
            if (absdiff(s1, s2) != 1) continue;          /* horizontal ok already, just double‑check */
            if (absdiff(s1, s1prev) != 1) continue;      /* vertical with previous block */
            if (absdiff(s2, s2prev) != 1) continue;
            int next_state = nb * STATE_S + s1 * SUMSIZE + s2;
            trans[state][trans_cnt[state]++] = (s16)next_state;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used but harmless */

    init_tables();

    /* DP arrays */
    static seq_t dp[STATES];
    static seq_t newdp[STATES];
    memset(dp, 0, sizeof(dp));

    /* initialise with the first two rows (rows 0 and 1) */
    for (int r0 = 0; r0 < STATE_ROWS(V, ROWS); ++r0) {
        for (int r1 = 0; r1 < STATE_ROWS(V, ROWS); ++r1) {
            if (!horiz_ok[r0][r1]) continue;
            int s1 = sum1[r0][r1];
            int s2 = sum2[r0][r1];
            int state = r1 * STATE_S + s1 * SUMSIZE + s2;
            dp[state] += (seq_t)1;
        }
    }

    /* number of rows is n+1; the first pair already used rows 0,1 */
    for (idx_t step = 2; step <= n; ++step) {
        memset(newdp, 0, sizeof(newdp));
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = dp[s];
            if (cnt == 0) continue;
            uint8_t tcnt = trans_cnt[s];
            for (uint8_t t = 0; t < tcnt; ++t) {
                int ns = trans[s][t];
                newdp[ns] += cnt;
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < STATES; ++i) dp[i] = newdp[i];
    }

    /* sum all states for the total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
