/*
 * A207804  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207804 – Number of n×4 binary arrays avoiding
 *  horizontal patterns 000 and 001, and vertical patterns 010 and 101.
 *  The implementation uses dynamic programming over the rows of the
 *  array.  For n = 1 and n = 2 the constraints are trivial; for n ≥ 3
 *  we keep a DP state consisting of the last two row patterns.
 *  The state space is 16 × 16 (all 4‑bit row patterns).
 *  The counts fit in seq_t; the algorithm works for arbitrary‑precision
 *  seq_t as well as for native integer sizes.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* horizontal constraint: a 4‑bit row must not contain
     * the 3‑bit subpatterns 000 or 001.
     */
    static bool allowed[16];
    static bool trans[16][16][16];
    static bool init = false;

    if (!init) {
        /* compute allowed row patterns */
        for (idx_t s = 0; s < 16; ++s) {
            bool ok = true;
            /* check two triples: bits (0,1,2) and (1,2,3) */
            for (idx_t i = 0; i < 2; ++i) {
                idx_t sub = (s >> i) & 7;   /* 3‑bit subpattern */
                if (sub == 0 || sub == 1)   /* 000 or 001 */
                    ok = false;
            }
            allowed[s] = ok;
        }

        /* precompute vertical transition between consecutive rows.
         * trans[u][v][w] is true iff the triple of rows
         * (u, v, w) satisfies the vertical constraints 010 and 101
         * for every column.
         */
        for (idx_t u = 0; u < 16; ++u)
            for (idx_t v = 0; v < 16; ++v)
                for (idx_t w = 0; w < 16; ++w) {
                    bool ok = true;
                    for (idx_t bit = 0; bit < 4; ++bit) {
                        idx_t a = (u >> bit) & 1U;
                        idx_t b = (v >> bit) & 1U;
                        idx_t c = (w >> bit) & 1U;
                        if (a == 0 && b == 1 && c == 0) { ok = false; break; }
                        if (a == 1 && b == 0 && c == 1) { ok = false; break; }
                    }
                    trans[u][v][w] = ok;
                }
        init = true;
    }

    if (n == 0)
        return (seq_t)0;

    /* For n = 1, only horizontal patterns matter. */
    if (n == 1) {
        seq_t sum = 0;
        for (idx_t s = 0; s < 16; ++s)
            if (allowed[s])
                sum += (seq_t)1;
        return sum;
    }

    /* DP arrays for the last two rows. */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];
    seq_t res = 0;

    /* Length 2: any pair of allowed rows is valid. */
    for (idx_t u = 0; u < 16; ++u)
        for (idx_t v = 0; v < 16; ++v)
            dp[u][v] = (allowed[u] && allowed[v]) ? (seq_t)1 : (seq_t)0;

    if (n == 2) {
        for (idx_t u = 0; u < 16; ++u)
            for (idx_t v = 0; v < 16; ++v)
                res += dp[u][v];
        return res;
    }

    /* Length ≥ 3 */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero ndp */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (idx_t u = 0; u < 16; ++u) if (allowed[u])
            for (idx_t v = 0; v < 16; ++v) if (allowed[v]) {
                seq_t cur = dp[u][v];
                if (cur == 0) continue;
                for (idx_t w = 0; w < 16; ++w) if (allowed[w]) {
                    if (trans[u][v][w])
                        ndp[v][w] = ndp[v][w] + cur;
                }
            }

        /* copy ndp -> dp */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    for (idx_t u = 0; u < 16; ++u)
        for (idx_t v = 0; v < 16; ++v)
            res += dp[u][v];

    return res;
}
