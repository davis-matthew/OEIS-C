/*
 * A234031  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0..4} with all 2×2
 *            subblocks having all six edge and diagonal differences ≤1.
 *            Computed via dynamic programming over the 4‑column row states
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int len;
    int next[625];                /* maximum possible outgoing edges */
} RowState;

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Global data (generated once)                                       */
    /* ------------------------------------------------------------------ */
    static int       digits[625][4];
    static bool      rowValid[625];
    static int       valIndex[625];
    static RowState  trans[625];
    static int       validCount = 0;
    static bool      initialized = false;

    if (!initialized) {
        /* enumerate all 4‑column rows over {0,1,2,3,4} */
        for (int s = 0; s < 625; ++s) {
            int t = s;
            for (int k = 3; k >= 0; --k) {      /* base‑5 digits */
                digits[s][k] = t % 5;
                t /= 5;
            }
            bool ok = true;
            for (int j = 0; j < 3; ++j) {       /* check horizontal adjacency */
                int d = digits[s][j] - digits[s][j+1];
                if (d < 0) d = -d;
                if (d > 1) { ok = false; break; }
            }
            if (ok) {
                rowValid[s] = true;
                valIndex[s] = validCount++;
            } else {
                rowValid[s] = false;
                valIndex[s] = -1;
            }
        }

        /* compute adjacency between valid rows */
        for (int s1 = 0; s1 < 625; ++s1) if (rowValid[s1]) {
            int i = valIndex[s1];
            int cnt = 0;
            for (int s2 = 0; s2 < 625; ++s2) if (rowValid[s2]) {
                bool adj = true;
                for (int j = 0; j < 3 && adj; ++j) {
                    int a1 = digits[s1][j],   a2 = digits[s1][j+1];
                    int b1 = digits[s2][j],   b2 = digits[s2][j+1];
                    int d;
                    d = a1 - b1; if (d < 0) d = -d; if (d > 1) {adj = false; break;}
                    d = a2 - b2; if (d < 0) d = -d; if (d > 1) {adj = false; break;}
                    d = a1 - b2; if (d < 0) d = -d; if (d > 1) {adj = false; break;}
                    d = a2 - b1; if (d < 0) d = -d; if (d > 1) {adj = false; break;}
                }
                if (adj) {
                    trans[i].next[cnt++] = valIndex[s2];
                }
            }
            trans[i].len = cnt;
        }
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* Dynamic programming over rows                                       */
    /* ------------------------------------------------------------------ */
    static seq_t cur[625];
    static seq_t nxt[625];
    seq_t *dp0 = cur;
    seq_t *dp1 = nxt;

    /* any first row is allowed – initialise with 1 */
    for (int i = 0; i < validCount; ++i) dp0[i] = 1;

    /* perform n transitions (array has n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        /* clear destination */
        for (int i = 0; i < validCount; ++i) dp1[i] = 0;

        /* propagate counts */
        for (int i = 0; i < validCount; ++i) {
            seq_t val = dp0[i];
            if (val == 0) continue;
            for (int t = 0; t < trans[i].len; ++t) {
                int j = trans[i].next[t];
                dp1[j] += val;
            }
        }

        /* swap pointers for next round */
        seq_t *tmp = dp0;
        dp0 = dp1;
        dp1 = tmp;
    }

    /* sum over all possible last rows */
    seq_t total = 0;
    for (int i = 0; i < validCount; ++i) total += dp0[i];
    return total;
}
