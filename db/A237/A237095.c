/*
 * A237095  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock equal.
 * https://oeis.org/A237095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A237095: Number of (n+1)×5 arrays over {0,1,2} whose
 * max + lower_median − upper_median of every 2×2 subblock is constant.
 * Offset = 1. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum number of row patterns: 3⁵ = 243 */
#define STATES 243
/* Number of distinct 2×2 patterns in a row pair: 3⁴ = 81 */
#define PATTERNS 81

/* ------------------------------------------------------------------ */
/* Pre‑computed tables                                                    */
static unsigned char rowDigits[STATES][5];   /* base‑3 digits of each row */
static unsigned char patternVal[PATTERNS];   /* pre‑computed block values */
static int          succ[3][STATES][STATES]; /* adjacency lists for each K */
static uint16_t     succ_cnt[3][STATES];     /* length of each list */

/* ------------------------------------------------------------------ */
/* Flag to avoid re‑initialising tables multiple times                */
static bool tables_initialized = false;

/* ------------------------------------------------------------------ */
/* Build the static tables on first usage                              */
static void init_tables(void)
{
    if (tables_initialized) return;

    /* 1. Compute pattern value for every 2×2 block (a,b,c,d in {0,1,2}) */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d) {
                    int vals[4] = { a, b, c, d };
                    /* sort ascending */
                    for (int i = 0; i < 4; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (vals[i] > vals[j]) {
                                int t = vals[i];
                                vals[i] = vals[j];
                                vals[j] = t;
                            }
                    /* value = max + lower median - upper median = v3 + v1 - v2 */
                    patternVal[a*27 + b*9 + c*3 + d] =
                        (unsigned char)(vals[3] + vals[1] - vals[2]); /* 0..2 */
                }

    /* 2. Convert each state integer [0,243) into 5 base‑3 digits */
    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        for (int col = 0; col < 5; ++col) {
            rowDigits[s][col] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* 3. Build adjacency lists for each constant K = 0,1,2 */
    for (int K = 0; K < 3; ++K)
        for (int i = 0; i < STATES; ++i) {
            uint16_t cnt = 0;
            for (int j = 0; j < STATES; ++j) {
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int a = rowDigits[i][col];
                    int b = rowDigits[i][col + 1];
                    int c = rowDigits[j][col];
                    int d = rowDigits[j][col + 1];
                    if (patternVal[a*27 + b*9 + c*3 + d] != K) {
                        ok = false;
                        break;
                    }
                }
                if (ok) succ[K][i][cnt++] = j;
            }
            succ_cnt[K][i] = cnt;
        }

    tables_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function required by the exercise                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset starts at 1 */

    init_tables();

    seq_t total = 0;

    /* Temporary DP arrays – small enough for the stack */
    static seq_t dp[STATES];
    static seq_t next[STATES];

    for (int K = 0; K < 3; ++K) {
        /* initialise dp: one way to start from every row pattern */
        for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;

        /* iterate over the n vertical transitions */
        for (idx_t step = 0; step < n; ++step) {
            /* zero next */
            for (int i = 0; i < STATES; ++i) next[i] = (seq_t)0;

            /* propagate from each current state */
            for (int i = 0; i < STATES; ++i) {
                seq_t val = dp[i];
                if (!val) continue;
                uint16_t cnt = succ_cnt[K][i];
                for (uint16_t s = 0; s < cnt; ++s) {
                    int j = succ[K][i][s];
                    next[j] += val;
                }
            }

            /* swap dp and next for the next step */
            for (int i = 0; i < STATES; ++i) dp[i] = next[i];
        }

        /* sum all counts for this K */
        seq_t sumK = 0;
        for (int i = 0; i < STATES; ++i) sumK += dp[i];
        total += sumK;
    }

    return total;
}
