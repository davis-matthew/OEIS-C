/*
 * A183459  Number of (n+4)X6 binary arrays with every 1 having exactly three
 *          king-move neighbors equal to 1 but with no 2X2 blocks of 1s.
 * https://oeis.org/A183459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183459: number of (n+4)×6 binary arrays in which every 1 has exactly
   three king‑move neighbours equal to 1 and the array contains no 2×2 block of
   1s.  The values are computed by dynamic programming over rows.  The
   algorithm has O(n·64²) time and O(64²) memory, which is easily fast enough
   for n up to 10 000. */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define NUM_PAT 64
#define NUM_STATE (NUM_PAT * NUM_PAT)

/* pre‑computed constraints */
static unsigned char no2x2[NUM_PAT][NUM_PAT];
static unsigned char transCount[NUM_STATE];
static unsigned char transNext[NUM_STATE][NUM_PAT];
static unsigned char selfLR[NUM_PAT][6];
static bool tables_built = false;

/* build lookup tables once */
static void init_tables(void)
{
    /* no 2×2 blocks between rows */
    for (int a = 0; a < NUM_PAT; ++a) {
        for (int b = 0; b < NUM_PAT; ++b) {
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                if ( ((a >> j)       & 1) &&
                     ((a >> (j + 1)) & 1) &&
                     ((b >> j)       & 1) &&
                     ((b >> (j + 1)) & 1) )
                    ok = false;
            }
            no2x2[a][b] = ok;
        }
    }

    /* left/right neighbours within a row */
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int j = 0; j < 6; ++j) {
            unsigned char cnt = 0;
            if (j > 0     && ((p >> (j - 1)) & 1)) ++cnt;
            if (j < 5     && ((p >> (j + 1)) & 1)) ++cnt;
            selfLR[p][j] = cnt;
        }
    }

    /* transitions: from (prev_prev,prev) to next row */
    for (int pp = 0; pp < NUM_PAT; ++pp) {
        for (int p = 0; p < NUM_PAT; ++p) {
            int st = (pp << 6) | p;
            if (!no2x2[pp][p]) {
                transCount[st] = 0;
                continue;
            }
            unsigned char sz = 0;
            for (int n = 0; n < NUM_PAT; ++n) {
                if (!no2x2[p][n]) continue;

                /* check every 1 in row p has exactly 3 king‑move neighbours */
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j) {
                    if ((p >> j) & 1) {
                        int c = selfLR[p][j]                       /* left/right */
                            + ((pp >> j) & 1)                      /* up same col */
                            + ((n  >> j) & 1);                     /* down same col */
                        if (j > 0) c += ((pp >> (j - 1)) & 1);     /* up left */
                        if (j < 5) c += ((pp >> (j + 1)) & 1);     /* up right */
                        if (j > 0) c += ((n >> (j - 1)) & 1);      /* down left */
                        if (j < 5) c += ((n >> (j + 1)) & 1);      /* down right */
                        if (c != 3) ok = false;
                    }
                }
                if (ok) {
                    transNext[st][sz++] = (unsigned char)n;
                }
            }
            transCount[st] = sz;
        }
    }
}

seq_t a(idx_t n)
{
    if (!tables_built) {
        init_tables();
        tables_built = true;
    }

    if (n == 0) return 0;   /* sequence starts at index 1 */

    idx_t m = n + 4;        /* number of rows */

    /* DP arrays: dp[prev_prev<<6 | prev]  */
    seq_t dp[NUM_STATE];
    seq_t ndp[NUM_STATE];

    /* explicit initialization to avoid calling memset on non‑trivial types */
    for (size_t i = 0; i < NUM_STATE; ++i) dp[i] = 0;
    for (size_t i = 0; i < NUM_STATE; ++i) ndp[i] = 0;
    dp[0] = 1;              /* state (0,0) before any rows */

/* main DP loop */
    for (idx_t step = 0; step < m; ++step) {
        for (size_t i = 0; i < NUM_STATE; ++i) ndp[i] = 0;
        for (int st = 0; st < NUM_STATE; ++st) {
            seq_t curcnt = dp[st];
            if (curcnt == 0) continue;
            unsigned char *nexts = transNext[st];
            unsigned char cnt = transCount[st];
            for (unsigned char k = 0; k < cnt; ++k) {
                int newrow = nexts[k];
                int newst  = ((st & 63) << 6) | newrow;
                ndp[newst] += curcnt;
            }
        }
        for (size_t i = 0; i < NUM_STATE; ++i) dp[i] = ndp[i];
    }

    /* final row constraint: bottom row has no below neighbours */
    seq_t ans = 0;
    for (int st = 0; st < NUM_STATE; ++st) {
        seq_t ways = dp[st];
        if (ways == 0) continue;
        int prev_prev = st >> 6;
        int prev      = st & 63;

        bool ok = true;
        for (int j = 0; j < 6 && ok; ++j) {
            if ((prev >> j) & 1) {
                int c = selfLR[prev][j] + ((prev_prev >> j) & 1);
                if (j > 0) c += ((prev_prev >> (j - 1)) & 1);
                if (j < 5) c += ((prev_prev >> (j + 1)) & 1);
                /* no below neighbours */
                if (c != 3) ok = false;
            }
        }
        if (ok) ans += ways;
    }

    return ans;
}
