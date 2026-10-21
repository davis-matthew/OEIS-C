/*
 * A204700  Number of nX3 0..2 arrays with no occurrence of three equal
 *          elements in a row horizontally, vertically or nw-to-se diagonally,
 *          and new values 0..2 introduced in row major order.
 * https://oeis.org/A204700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function implements the definition of OEIS A204700. It uses
 *            dynamic programming over rows. The recurrence is not a closed
 *            form; the DP tracks the current and previous two rows of a
 *            3‑column array and the possession of the 1–or–2 symbols to
 *            enforce the "row‑major introduction" rule
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   mask_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mask_index.h"

/* ------------------------------------------------------------------ */
/* Types supplied by the build                                            */
/* ------------------------------------------------------------------ */
/* seq_t and idx_t are defined elsewhere. */

/* ------------------------------------------------------------------ */
/* Constants and helper structures                                      */
/* ------------------------------------------------------------------ */
#define MAX_PAT 27          /* 3^3 possible 3‑tuples */
typedef struct {
    unsigned char v[3];      /* values in the row */
    bool has1;               /* contains value 1? */
    bool has2;               /* contains value 2? */
    bool badBefore1;         /* 2 appears before 1 in this tuple */
} Pattern;

static Pattern patterns[MAX_PAT];
static int patCnt = 0;              /* actual number of patterns used */

/* validMask[mask][pat] : true iff pattern pat is allowed when the
   mask indicates the symbols seen before this row.  masks:
   0 = none, 1 = 1 seen, 2 = 2 seen (cannot happen), 3=both seen */
static bool validMask[4][MAX_PAT];

/* nextMask[a][b] : bit set if pattern c can follow a,b
   respecting vertical and NW→SE diagonal triple constraints. */
static uint32_t nextMask[MAX_PAT][MAX_PAT];

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
   /* 0..3 */

/* ------------------------------------------------------------------ */
/* Precomputation of patterns and transition tables                  */
/* ------------------------------------------------------------------ */
static void init_tables(void) {
    if (patCnt) return;                    /* already initialized */

    /* Enumerate all 3‑tuples that are not all equal */
    for (unsigned a = 0; a < 3; ++a)
        for (unsigned b = 0; b < 3; ++b)
            for (unsigned c = 0; c < 3; ++c) {
                if (a == b && b == c) continue;   /* horizontal triple forbidden */
                Pattern p;
                p.v[0] = a; p.v[1] = b; p.v[2] = c;
                p.has1 = (a == 1 || b == 1 || c == 1);
                p.has2 = (a == 2 || b == 2 || c == 2);
                /* Determine if a 2 appears before any 1 in this tuple */
                bool seen1 = false;
                p.badBefore1 = false;
                for (int i = 0; i < 3; ++i) {
                    if (p.v[i] == 1) seen1 = true;
                    if (p.v[i] == 2 && !seen1) p.badBefore1 = true;
                }
                patterns[patCnt++] = p;
            }
    /* patCnt should be 24 */

    /* Build validMask table.  A pattern may be used when
       the previous mask has 1, or the pattern itself has no illegal
       2-before-1 situation and the mask does not forbid 2. */
    for (int mask = 0; mask < 4; ++mask) {
        bool has1 = mask & 1;
        for (int i = 0; i < patCnt; ++i) {
            if (has1) {
                validMask[mask][i] = true;
            } else {
                /* mask has no 1 seen yet */
                if (patterns[i].badBefore1) validMask[mask][i] = false;
                else {
                    /* If pattern has 2 but no 1, mask cannot allow it */
                    if (patterns[i].has2 && !patterns[i].has1) {
                        validMask[mask][i] = false;
                    } else {
                        validMask[mask][i] = true;
                    }
                }
            }
        }
    }

    /* Build nextMask table with vertical and diagonal constraints. */
    for (int a = 0; a < patCnt; ++a) {
        for (int b = 0; b < patCnt; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < patCnt; ++c) {
                bool bad = false;
                /* Vertical triple */
                for (int col = 0; col < 3; ++col)
                    if (patterns[a].v[col] == patterns[b].v[col] &&
                        patterns[b].v[col] == patterns[c].v[col]) {
                        bad = true; break;
                    }
                /* NW→SE diagonal */
                if (!bad)
                    if (patterns[a].v[0] == patterns[b].v[1] &&
                        patterns[b].v[1] == patterns[c].v[2])
                        bad = true;
                if (!bad) mask |= (1U << c);
            }
            nextMask[a][b] = mask;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n < 1 */

    init_tables();

    /* Count valid first‑row patterns (first cell is 0). */
    static seq_t firstRowCount = 0;
    if (!firstRowCount) {
        for (int i = 0; i < patCnt; ++i)
            if (patterns[i].v[0] == 0 && validMask[0][i])
                ++firstRowCount;
    }
    if (n == 1) return firstRowCount;

    /* DP arrays: dp[cur][prevRow][curRow][maskIdx] */
    static seq_t dp[2][MAX_PAT][MAX_PAT][4];
    int cur = 0, nxt = 1;
    memset(dp, 0, sizeof(dp));

    /* Initialise DP for the first two rows (rows 1 and 2). */
    for (int i = 0; i < patCnt; ++i) {
        if (patterns[i].v[0] != 0) continue;          /* first cell must be 0 */
        if (!validMask[0][i]) continue;               /* check mask before row 0 */
        int mask1 = (patterns[i].has1 ? 1 : 0) | (patterns[i].has2 ? 2 : 0);
        int idx1 = mask_index(mask1);
        for (int j = 0; j < patCnt; ++j) {
            if (!validMask[idx1][j]) continue;        /* check mask before row 1 */
            int mask2 = mask1 | (patterns[j].has1 ? 1 : 0)
                           | (patterns[j].has2 ? 2 : 0);
            /* mask2 cannot be 2 without 1; such states are unreachable because
               validMask ensures patterns with isolated 2 are forbidden when
               mask1 lacks 1. */
            int idx2 = mask_index(mask2);
            dp[cur][i][j][idx2] += (seq_t)1;
        }
    }
    if (n == 2) {
        seq_t total = 0;
        for (int a = 0; a < patCnt; ++a)
            for (int b = 0; b < patCnt; ++b)
                for (int m = 0; m < 4; ++m)
                    total += dp[cur][a][b][m];
        return total;
    }

    /* Process remaining rows (row 3 up to n). */
    for (idx_t row = 3; row <= n; ++row) {
        memset(dp[nxt], 0, sizeof(dp[0]));
        for (int a = 0; a < patCnt; ++a)
            for (int b = 0; b < patCnt; ++b)
                for (int mIdx = 0; mIdx < 4; ++mIdx) {
                    seq_t cnt = dp[cur][a][b][mIdx];
                    if (!cnt) continue;
                    uint32_t allowed = nextMask[a][b];
                    while (allowed) {
                        int c = __builtin_ctz(allowed);
                        allowed &= allowed - 1;          /* clear lowest bit */
                        if (!validMask[mIdx][c]) continue;
                        int newMask = mIdx | (patterns[c].has1 ? 1 : 0)
                                         | (patterns[c].has2 ? 2 : 0);
                        /* newMask cannot become 2 without 1 because validMask
                           forbids patterns with isolated 2 when previous mask
                           lacks 1. */
                        int newIdx = mask_index(newMask);
                        dp[nxt][b][c][newIdx] += cnt;
                    }
                }
        /* swap buffers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all states in the final row */
    seq_t result = 0;
    for (int a = 0; a < patCnt; ++a)
        for (int b = 0; b < patCnt; ++b)
            for (int m = 0; m < 4; ++m)
                result += dp[cur][a][b][m];

    return result;
}
