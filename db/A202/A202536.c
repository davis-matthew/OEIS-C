/*
 * A202536  Number of tilings of a 4 X n rectangle using straight (3 X 1)
 *          trominoes and 2 X 2 tiles.
 * https://oeis.org/A202536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 63, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = number of tilings of a 4×n rectangle with
   straight (3×1) trominoes and 2×2 squares.
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=3.  */
#include <stdint.h>          /* For uint64_t used by the build system */

#define STATES 256
#define MAX_TRANS 64

static unsigned char  transDest[STATES][MAX_TRANS];
static unsigned short transCnt[STATES][MAX_TRANS];
static unsigned char  transSize[STATES] = {0};
static int            initialized       = 0;

/* Recursive construction of all column‑to‑column transitions for a
   given (current mask, next mask) state.
   The localCnt array accumulates the number of ways to reach each
   destination state. */
static void dfs_fill(int cur_mask, int next_mask, int nextnext_mask,
                     int localCnt[STATES]) {
    /* Current column fully filled – record transition */
    if (cur_mask == 0xF) {
        int dest = next_mask | (nextnext_mask << 4);
        localCnt[dest]++;
        return;
    }

    /* Find first empty cell (row r) in the current column */
    int r = 0;
    while (cur_mask & (1 << r)) ++r;          /* r < 4 guaranteed */

    /* 1. Vertical tromino (3×1) starting at row r */
    if (r <= 1 &&
        !(cur_mask & (1 << (r + 1))) &&
        !(cur_mask & (1 << (r + 2)))) {
        int cur2 = cur_mask | (1 << r) | (1 << (r + 1)) | (1 << (r + 2));
        dfs_fill(cur2, next_mask, nextnext_mask, localCnt);
    }

    /* 2. 2×2 tile covering rows r,r+1 in current and next columns */
    if (r <= 2 &&
        !(cur_mask & (1 << (r + 1))) &&
        !(next_mask & (1 << r)) &&
        !(next_mask & (1 << (r + 1)))) {
        int cur2  = cur_mask | (1 << r) | (1 << (r + 1));
        int next2 = next_mask | (1 << r) | (1 << (r + 1));
        dfs_fill(cur2, next2, nextnext_mask, localCnt);
    }

    /* 3. Horizontal tromino (1×3) starting at row r */
    if (!(next_mask & (1 << r)) && !(nextnext_mask & (1 << r))) {
        int cur2      = cur_mask | (1 << r);
        int next2     = next_mask | (1 << r);
        int nextnext2 = nextnext_mask | (1 << r);
        dfs_fill(cur2, next2, nextnext2, localCnt);
    }
}

/* Build all transition tables once */
static void build_transitions(void) {
    if (initialized) return;
    for (int cur = 0; cur < 16; ++cur) {
        for (int next = 0; next < 16; ++next) {
            int state = cur | (next << 4);
            int localCnt[STATES] = {0};
            dfs_fill(cur, next, 0, localCnt);
            unsigned char sz = 0;
            for (int d = 0; d < STATES; ++d) {
                if (localCnt[d]) {
                    transDest[state][sz] = (unsigned char)d;
                    transCnt[state][sz] = (unsigned short)localCnt[d];
                    ++sz;
                }
            }
            transSize[state] = sz;
        }
    }
    initialized = 1;
}

/* Sequence function.  External linkage is required by the build system. */
seq_t a(idx_t n) {
    build_transitions();
    seq_t dp[STATES] = {0};
    seq_t ndp[STATES] = {0};
    dp[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        /* clear next DP state */
        for (int k = 0; k < STATES; ++k) ndp[k] = 0;
        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == 0) continue;
            unsigned char sz = transSize[s];
            for (unsigned char t = 0; t < sz; ++t) {
                int dest = transDest[s][t];
                seq_t ways = (seq_t)transCnt[s][t];
                ndp[dest] += dp[s] * ways;
            }
        }
        /* advance to next column */
        for (int k = 0; k < STATES; ++k) dp[k] = ndp[k];
    }
    return dp[0];  /* state (0,0) – no hanging tiles */
}
