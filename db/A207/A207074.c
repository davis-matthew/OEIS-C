/*
 * A207074  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207074: Number of 7×n 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally and
   0 0 1 and 1 0 1 vertically.  Computed by a DP over columns with state
   equal to the last two columns. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static const int M = 14;                     /* number of admissible 7‑bit columns */
static int  allowed[14];                     /* masks of admissible columns   */
static char tripleValid[14][14][14];         /* 1 if columns i,j,k satisfy horizontal rules */
static bool init_done = false;               /* initialization flag            */

/* initialise the tables of allowed columns and horizontal triples */
static void init_tables(void)
{
    int cnt = 0;
    for (int mask = 0; mask < 128; ++mask) {
        bool ok = true;
        /* vertical patterns inside a column: 001 or 101 */
        for (int r = 0; r <= 4; ++r) {
            int pat = ((mask >> (r + 2)) & 1) << 2 |
                      ((mask >> (r + 1)) & 1) << 1 |
                      ((mask >> r) & 1);
            if (pat == 1 || pat == 5) {          /* 001 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) {
            allowed[cnt++] = mask;
        }
    }

    /* precompute horizontal validity of triples of columns */
    for (int i = 0; i < cnt; ++i) {
        for (int j = 0; j < cnt; ++j) {
            for (int k = 0; k < cnt; ++k) {
                bool good = true;
                for (int r = 0; r < 7; ++r) {
                    int a = (allowed[i] >> r) & 1;
                    int b = (allowed[j] >> r) & 1;
                    int c = (allowed[k] >> r) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 0 && b == 1 && c == 0)) {
                        good = false;
                        break;
                    }
                }
                tripleValid[i][j][k] = good ? 1 : 0;
            }
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (!init_done) init_tables();

    if (n == 0) return (seq_t)0;               /* out of range, harmless */

    /* trivial base cases */
    if (n == 1) return (seq_t)M;                /* any single admissible column */
    if (n == 2) return (seq_t)M * (seq_t)M;    /* any pair of admissible columns */

    /* DP arrays: dp[i][j] = number of ways ending with columns i,j */
    static seq_t dp0[196];
    static seq_t dp1[196];
    seq_t *cur  = dp0;
    seq_t *next = dp1;
    const idx_t pairSize = (idx_t)M * (idx_t)M;

    /* initialise for the first two columns: all pairs are admissible */
    for (idx_t idx = 0; idx < pairSize; ++idx) {
        cur[idx] = 1;
    }

    /* iterate from column 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero the next DP array */
        for (idx_t idx = 0; idx < pairSize; ++idx) {
            next[idx] = 0;
        }

        /* transition */
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t val = cur[(idx_t)i * (idx_t)M + (idx_t)j];
                if (val == 0) continue;
                for (int k = 0; k < M; ++k) {
                    if (tripleValid[i][j][k]) {
                        next[(idx_t)j * (idx_t)M + (idx_t)k] += val;
                    }
                }
            }
        }

        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all counts for the last two columns */
    seq_t result = 0;
    for (idx_t idx = 0; idx < pairSize; ++idx) {
        result += cur[idx];
    }
    return result;
}
