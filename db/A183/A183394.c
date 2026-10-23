/*
 * A183394  Half the number of nX3 binary arrays with no element equal to a
 *          strict majority of its knight-move neighbors.
 * https://oeis.org/A183394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * (# of n×3 binary arrays with no element equal to
 *            a strict majority of its knight‑move neighbors)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* -------------  Constants  ----------------- */

#define ABSENT   8               /* sentinel for missing rows */
#define STATE_BASE 9             /* 8 real rows + 1 sentinel */
#define STATE_SIZE (STATE_BASE*STATE_BASE*STATE_BASE*STATE_BASE) /* 9^4 = 6561 */
#define RANKS     3              /* number of columns */

#define NR 8                   /* knight neighbours */
static const int DR[NR] = {-2,-2,-1,-1, 1, 1, 2, 2};
static const int DC[NR] = {-1, 1,-2, 2,-2, 2,-1, 1};

/* -------------  Helpers  ------------------- */

/* Encode four rows into a single state index (0 .. 6560) */
static inline int encode(int a, int b, int c, int d) {
    return (((a*STATE_BASE + b)*STATE_BASE + c)*STATE_BASE + d);
}

/* Decode a state into four rows */
static inline void decode(int state, int *rows) {
    int d = state % STATE_BASE; state /= STATE_BASE;
    int c = state % STATE_BASE; state /= STATE_BASE;
    int b = state % STATE_BASE; state /= STATE_BASE;
    int a = state;
    rows[0] = a; rows[1] = b; rows[2] = c; rows[3] = d;
}

/* Return true if the middle row (index 2) satisfies the majority rule */
static bool valid_middle(const int *row5) {
    for (int col = 0; col < RANKS; ++col) {
        int v   = (row5[2] >> col) & 1;
        int k   = 0;
        int same = 0;
        for (int m = 0; m < NR; ++m) {
            int r = DR[m] + 2;          /* map -2,-1,1,2 to indices 0,1,3,4 */
            if (row5[r] == ABSENT) continue;
            int nc = col + DC[m];
            if (nc < 0 || nc >= RANKS) continue;
            int nv = (row5[r] >> nc) & 1;
            if (nv == v) ++same;
            ++k;
        }
        if (same * 2 > k) return false;  /* strict majority found */
    }
    return true;
}

/* -------------  Global tables  -------------- */

static bool tables_inited = false;
static int trans[STATE_SIZE][8];
static unsigned char transCnt[STATE_SIZE];
static bool sentinelOk[STATE_SIZE];

/* -------------  Precomputation  ------------- */

static void build_tables(void) {
    int rows[4];
    int five[5];

    for (int st = 0; st < STATE_SIZE; ++st) {
        decode(st, rows);

        /* transitions to real rows 0..7 */
        int cnt = 0;
        for (int r = 0; r < 8; ++r) {
            five[0] = rows[0]; five[1] = rows[1];
            five[2] = rows[2]; five[3] = rows[3]; five[4] = r;
            if (valid_middle(five))
                trans[st][cnt++] = r;
        }
        transCnt[st] = (unsigned char)cnt;

        /* sentinel transition (next row = ABSENT) */
        five[0] = rows[0]; five[1] = rows[1];
        five[2] = rows[2]; five[3] = rows[3]; five[4] = ABSENT;
        sentinelOk[st] = valid_middle(five);
    }
    tables_inited = true;
}

/* -------------  Main function  -------------- */

seq_t a(idx_t n) {
    /* Special cases not covered by the DP algorithm */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Initialise tables on first call */
    if (!tables_inited) build_tables();

    /* DP arrays */
    static seq_t dp[STATE_SIZE];
    static seq_t nxt[STATE_SIZE];

    /* Zero dp */
    memset(dp, 0, sizeof(dp));

    /* Initial states: two sentinel rows before the first real row */
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2)
            dp[encode(ABSENT, ABSENT, r1, r2)] = 1;

    /* Handle rows 3 .. n */
    for (idx_t rowIdx = 3; rowIdx <= n; ++rowIdx) {
        memset(nxt, 0, sizeof(nxt));
        for (int st = 0; st < STATE_SIZE; ++st) {
            if (dp[st] == 0) continue;
            int rem = st % 729;                 /* remainder = state % 9^3 */
            seq_t cur = dp[st];
            unsigned char c = transCnt[st];
            for (unsigned char j = 0; j < c; ++j) {
                int newSt = rem * 9 + trans[st][j];
                nxt[newSt] += cur;
            }
        }
        memcpy(dp, nxt, sizeof(dp));
    }

    /* Close with two sentinel rows */
    for (int step = 0; step < 2; ++step) {
        memset(nxt, 0, sizeof(nxt));
        for (int st = 0; st < STATE_SIZE; ++st) {
            if (dp[st] == 0) continue;
            if (!sentinelOk[st]) continue;
            int rem = st % 729;
            int newSt = rem * 9 + ABSENT;
            nxt[newSt] += dp[st];
        }
        memcpy(dp, nxt, sizeof(dp));
    }

    /* Sum all final counts */
    seq_t total = 0;
    for (int st = 0; st < STATE_SIZE; ++st) total += dp[st];

    /* Return half of the count */
    return total / 2;
}
