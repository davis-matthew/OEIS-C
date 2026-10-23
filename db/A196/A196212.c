/*
 * A196212  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,1,2,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A196212: number of n×2 arrays with elements 0..4 such that each cell x has exactly x
   neighbours equal to MAP[x] where MAP = {3,0,1,2,4} among horizontal and vertical
   neighbours.  The algorithm builds all valid rows and propagates them row by row
   using pre‑computed transitions for interior rows. */
#include <stdbool.h>

enum { STATE_COUNT = 25 };                /* 5² possible rows for a 2­column table */
static int MAP[5] = {3, 0, 1, 2, 4};

/* Transition tables for interior rows: NEXT[a][b][k] = c means that
   if row "b" has above row "a", then "c" can be the next row. */
static int NEXT[STATE_COUNT][STATE_COUNT][STATE_COUNT];
static uint8_t NEXT_CNT[STATE_COUNT][STATE_COUNT];
static bool INIT_DONE = false;

/* Decode a packed row state (0..24) into its two cell values. */
static inline void decode(int state, int *v0, int *v1) {
    *v0 = state / 5;
    *v1 = state % 5;
}

/* Check if a row is valid when it has both an above and a below neighbour. */
static bool valid_internal(int row, int above, int below) {
    int v0, v1, a0, a1, b0, b1;
    decode(row, &v0, &v1);
    decode(above, &a0, &a1);
    decode(below, &b0, &b1);

    int cnt0 = 0;
    if (a0 == MAP[v0]) ++cnt0;
    if (b0 == MAP[v0]) ++cnt0;
    if (v1 == MAP[v0]) ++cnt0;
    if (cnt0 != v0) return false;

    int cnt1 = 0;
    if (a1 == MAP[v1]) ++cnt1;
    if (b1 == MAP[v1]) ++cnt1;
    if (v0 == MAP[v1]) ++cnt1;
    if (cnt1 != v1) return false;

    return true;
}

/* Validate the top row (no above neighbours). */
static bool valid_top(int row, int below) {
    int v0, v1, b0, b1;
    decode(row, &v0, &v1);
    decode(below, &b0, &b1);

    int cnt0 = 0;
    if (b0 == MAP[v0]) ++cnt0;
    if (v1 == MAP[v0]) ++cnt0;
    if (cnt0 != v0) return false;

    int cnt1 = 0;
    if (b1 == MAP[v1]) ++cnt1;
    if (v0 == MAP[v1]) ++cnt1;
    if (cnt1 != v1) return false;

    return true;
}

/* Validate the bottom row (no below neighbours). */
static bool valid_bottom(int row, int above) {
    int v0, v1, a0, a1;
    decode(row, &v0, &v1);
    decode(above, &a0, &a1);

    int cnt0 = 0;
    if (a0 == MAP[v0]) ++cnt0;
    if (v1 == MAP[v0]) ++cnt0;
    if (cnt0 != v0) return false;

    int cnt1 = 0;
    if (a1 == MAP[v1]) ++cnt1;
    if (v0 == MAP[v1]) ++cnt1;
    if (cnt1 != v1) return false;

    return true;
}

/* Validate a single row with no neighbours (n = 1). */
static bool valid_single(int row) {
    int v0, v1;
    decode(row, &v0, &v1);

    if (v0 != 0 && v0 != 1) return false;
    if (v1 != 0 && v1 != 1) return false;

    int h0 = (v1 == MAP[v0]) ? 1 : 0;
    int h1 = (v0 == MAP[v1]) ? 1 : 0;
    return (h0 == v0) && (h1 == v1);
}

/* Pre‑compute the transition tables for interior rows. */
static void init_transitions(void) {
    int a, b, c;
    for (a = 0; a < STATE_COUNT; ++a) {
        for (b = 0; b < STATE_COUNT; ++b) {
            uint8_t cnt = 0;
            for (c = 0; c < STATE_COUNT; ++c) {
                if (valid_internal(b, a, c)) {
                    NEXT[a][b][cnt++] = c;
                }
            }
            NEXT_CNT[a][b] = cnt;
        }
    }
    INIT_DONE = true;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the sequence, just return 0 */

    if (!INIT_DONE) init_transitions();

    /* n == 1 : handle separately */
    if (n == 1) {
        seq_t res = 0;
        for (int state = 0; state < STATE_COUNT; ++state) {
            if (valid_single(state)) res += 1;
        }
        return res;
    }

    /* DP tables: dp[i][j] counts ways where last two rows are (i, j). */
    seq_t dp[STATE_COUNT][STATE_COUNT];
    seq_t newdp[STATE_COUNT][STATE_COUNT];

    /* initialise dp with the first two rows, checking the top row. */
    for (int i = 0; i < STATE_COUNT; ++i)
        for (int j = 0; j < STATE_COUNT; ++j)
            dp[i][j] = 0;

    for (int s1 = 0; s1 < STATE_COUNT; ++s1) {
        for (int s2 = 0; s2 < STATE_COUNT; ++s2) {
            if (valid_top(s1, s2))
                dp[s1][s2] += 1;
        }
    }

    if (n == 2) {
        seq_t res = 0;
        for (int a = 0; a < STATE_COUNT; ++a) {
            for (int b = 0; b < STATE_COUNT; ++b) {
                if (dp[a][b] && valid_bottom(b, a))
                    res += dp[a][b];
            }
        }
        return res;
    }

    /* Process rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero newdp */
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                newdp[i][j] = 0;

        for (int a = 0; a < STATE_COUNT; ++a) {
            for (int b = 0; b < STATE_COUNT; ++b) {
                seq_t cur = dp[a][b];
                if (!cur) continue;
                uint8_t cnt = NEXT_CNT[a][b];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int c = NEXT[a][b][k];
                    newdp[b][c] += cur;
                }
            }
        }

        /* copy newdp into dp */
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After all rows, validate the bottom row. */
    seq_t res = 0;
    for (int a = 0; a < STATE_COUNT; ++a) {
        for (int b = 0; b < STATE_COUNT; ++b) {
            if (dp[a][b] && valid_bottom(b, a))
                res += dp[a][b];
        }
    }
    return res;
}
