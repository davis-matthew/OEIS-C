/*
 * A207427  Number of 3Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(1) = 6 a(2) = 36 for n >= 3: a(n) = sum_{s=0}^{35}
 *            dp[n][s] where dp[2][s] = 1 for every state s (pair of allowed
 *            columns) and dp[i+1][t] = sum_{s} dp[i][s] over all transitions
 *            s -> t States are pairs of allowed columns (prev1,prev0) with 6
 *            possible columns: 0,3,4,5,6,7. Transitions...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static const idx_t ALLOWED[6] = {0, 3, 4, 5, 6, 7};

seq_t a(idx_t n)
{
    /* quick answers for small n */
    if (n == 0) return (seq_t)0;        /* undefined for OEIS offset */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    /* --- transition tables ------------------------------------ */
    enum { AL = 6, ST = AL * AL };   /* AL == 6, ST == 36 */
    static int trans_count[ST] = {0};
    static int trans_list[ST][AL] = {{0}};   /* up to 6 targets per state */
    static bool init = false;

    if (!init) {
        /* pre‑compute allowed transitions */
        for (idx_t p1 = 0; p1 < AL; ++p1)                  /* prev1 column */
            for (idx_t p0 = 0; p0 < AL; ++p0) {           /* prev0 column */
                idx_t src = p1 * AL + p0;
                for (idx_t cur = 0; cur < AL; ++cur) {    /* new column */
                    idx_t c1 = ALLOWED[p1];
                    idx_t c0 = ALLOWED[p0];
                    idx_t c2 = ALLOWED[cur];
                    bool ok = true;
                    for (int r = 0; r < 3; ++r) {
                        int v = ((c1 >> r) & 1) << 2 |
                                ((c0 >> r) & 1) << 1 |
                                ((c2 >> r) & 1);
                        if (v == 0 || v == 2) { /* 000 or 010 forbidden */
                            ok = false; break;
                        }
                    }
                    if (!ok) continue;
                    idx_t dst = p0 * AL + cur;
                    trans_list[src][trans_count[src]++] = (int)dst;
                }
            }
        init = true;
    }

    /* DP arrays: current and next length i */
    seq_t cur[ST];
    seq_t nxt[ST];
    /* initialise dp for i = 2 : every pair of allowed columns has 1 way */
    for (size_t i = 0; i < ST; ++i) cur[i] = (seq_t)1;

    /* perform transitions for columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < ST; ++i) nxt[i] = (seq_t)0;
        for (size_t s = 0; s < ST; ++s) {
            if (cur[s] == (seq_t)0) continue;
            int tnum = trans_count[s];
            for (int k = 0; k < tnum; ++k) {
                int t = trans_list[s][k];
                nxt[t] += cur[s];
            }
        }
        /* copy nxt to cur */
        for (size_t i = 0; i < ST; ++i) cur[i] = nxt[i];
    }

    /* sum over all end states to obtain a(n) */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < ST; ++i) total += cur[i];
    return total;
}
