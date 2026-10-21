/*
 * A203096  Number of nX3 0..3 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of n × 3 arrays over {0,1,2,3} where every non‑zero
 *            element is ≤ at least one horizontal or vertical neighbor. This
 *            is done by a dynamic‑programming over row patterns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence defined for n ≥ 1 */

    /* ----------  precompute pattern helpers  ---------- */
    static bool initialized = false;
    static uint8_t val[64][3];
    static uint8_t unsatH[64];                /* unsatisfied horizontally */
    static uint8_t leq[64][64];              /* p[i] <= q[i] ? 1:0 */
    static uint8_t geq[64][64];              /* p[i] >= q[i] ? 1:0 */

    if (!initialized) {
        for (uint8_t p = 0; p < 64; ++p) {
            val[p][0] =  p           & 3;
            val[p][1] = (p >> 2)     & 3;
            val[p][2] = (p >> 4)     & 3;

            uint8_t h = 0;
            for (uint8_t c = 0; c < 3; ++c) {
                uint8_t v = val[p][c];
                if (v == 0) continue;            /* zeros do not need a neighbour */
                bool left_ok  = (c > 0)  && (val[p][c-1] >= v);
                bool right_ok = (c < 2)  && (val[p][c+1] >= v);
                if (!left_ok && !right_ok) h |= (1 << c);
            }
            unsatH[p] = h;
        }

        for (uint8_t p = 0; p < 64; ++p)
            for (uint8_t q = 0; q < 64; ++q) {
                uint8_t le = 0, ge = 0;
                for (uint8_t c = 0; c < 3; ++c) {
                    if (val[p][c] <= val[q][c]) le |= (1 << c);
                    if (val[p][c] >= val[q][c]) ge |= (1 << c);
                }
                leq[p][q] = le;
                geq[p][q] = ge;
            }
        initialized = true;
    }

    /* ----------  DP arrays  ---------- */
    seq_t dp[64][8];
    seq_t next[64][8];

    /* initialise to zero */
    for (uint8_t i = 0; i < 64; ++i)
        for (uint8_t m = 0; m < 8; ++m)
            dp[i][m] = 0;

    /* first row: no vertical neighbour, so unsatisfied mask is unsatH */
    for (uint8_t p = 0; p < 64; ++p)
        dp[p][unsatH[p]] = (seq_t)1;

    /* intermediate rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (uint8_t i = 0; i < 64; ++i)
            for (uint8_t m = 0; m < 8; ++m)
                next[i][m] = 0;

        for (uint8_t p = 0; p < 64; ++p) {
            for (uint8_t M = 0; M < 8; ++M) {
                seq_t cur = dp[p][M];
                if (cur == 0) continue;

                for (uint8_t q = 0; q < 64; ++q) {
                    /* check that all unsatisfied cells in p
                       receive a vertical neighbour >= their value */
                    if ((M & ~leq[p][q]) != 0) continue;

                    /* compute unsatisfied mask for q after considering
                       vertical neighbour from p */
                    uint8_t N = unsatH[q] & ~geq[p][q];
                    next[q][N] += cur;
                }
            }
        }
        /* copy next into dp for next iteration */
        for (uint8_t i = 0; i < 64; ++i)
            for (uint8_t m = 0; m < 8; ++m)
                dp[i][m] = next[i][m];
    }

    /* after n rows, only states with no unsatisfied cells survive */
    seq_t result = 0;
    for (uint8_t p = 0; p < 64; ++p)
        result += dp[p][0];

    return result;
}
