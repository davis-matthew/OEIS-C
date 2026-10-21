/*
 * A237335  Number of (n+1)X(3+1) 0..3 arrays with the maximum minus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A237335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} in which * all
 *            2×2 sub‑blocks have the same value of (max–min). The offset is *
 *            1: a(1) is the first term listed in the OEIS entry. The function
 *            * computes the n‑th term for any n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   build_graph
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/build_graph.h"

seq_t a(idx_t n)
{
    /* lazy static initialization of the transition graph */
    static int initialized = 0;
    static uint16_t neigh[256][256];
    static uint8_t  diff_arr[256][256];
    static uint16_t deg[256];
    if (!initialized) {
        build_graph(neigh, diff_arr, deg);
        initialized = 1;
    }

    /* DP: dp_cur[row][d] = number of valid prefixes ending with
     * the given last row and having the common difference d. */
    static seq_t dp_cur[256][4];
    static seq_t dp_next[256][4];

    /* clear current DP table */
    for (uint16_t i = 0; i < 256; ++i)
        for (uint8_t d = 0; d < 4; ++d)
            dp_cur[i][d] = 0;

    /* first transition: from any first row to a second row */
    for (uint16_t i = 0; i < 256; ++i) {
        uint16_t di = deg[i];
        for (uint16_t k = 0; k < di; ++k) {
            uint16_t j = neigh[i][k];
            uint8_t  d = diff_arr[i][k];
            dp_cur[j][d] += 1;
        }
    }

    /* remaining transitions (total rows = n+1, we already handled 2 rows) */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next table */
        for (uint16_t i = 0; i < 256; ++i)
            for (uint8_t d = 0; d < 4; ++d)
                dp_next[i][d] = 0;

        /* propagate */
        for (uint16_t i = 0; i < 256; ++i) {
            for (uint8_t d = 0; d < 4; ++d) {
                seq_t val = dp_cur[i][d];
                if (!val) continue;
                uint16_t di = deg[i];
                for (uint16_t k = 0; k < di; ++k) {
                    if (diff_arr[i][k] != d) continue;
                    uint16_t j = neigh[i][k];
                    dp_next[j][d] += val;
                }
            }
        }

        /* move next to current */
        for (uint16_t i = 0; i < 256; ++i)
            for (uint8_t d = 0; d < 4; ++d)
                dp_cur[i][d] = dp_next[i][d];
    }

    /* sum all ending states */
    seq_t ans = 0;
    for (uint16_t i = 0; i < 256; ++i)
        for (uint8_t d = 0; d < 4; ++d)
            ans += dp_cur[i][d];

    return ans;
}
