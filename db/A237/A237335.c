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
 * Validated: standard, pass, 28 terms through n = 203 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void build_graph(uint16_t neigh[256][256],
                        uint8_t  diff_arr[256][256],
                        uint16_t deg[256])
{
    for (uint32_t i = 0; i < 256; ++i) {
        /* decompose i into 4 digits base‑4 */
        uint8_t rowA[4];
        uint32_t x = i;
        for (int k = 3; k >= 0; --k) { rowA[k] = x % 4; x /= 4; }

        deg[i] = 0;
        for (uint32_t j = 0; j < 256; ++j) {
            uint8_t rowB[4];
            uint32_t y = j;
            for (int k = 3; k >= 0; --k) { rowB[k] = y % 4; y /= 4; }

            unsigned d = 0xFF;   /* use 0xFF as "invalid" marker */
            for (int col = 0; col <= 2; ++col) {
                /* compute max-min over the 2×2 block */
                unsigned mv = rowA[col];
                if (rowA[col+1] < mv) mv = rowA[col+1];
                if (rowB[col]   < mv) mv = rowB[col];
                if (rowB[col+1] < mv) mv = rowB[col+1];

                unsigned Mv = rowA[col];
                if (rowA[col+1] > Mv) Mv = rowA[col+1];
                if (rowB[col]   > Mv) Mv = rowB[col];
                if (rowB[col+1] > Mv) Mv = rowB[col+1];

                unsigned diff = Mv - mv;
                if (col == 0) d = diff;
                else if (diff != d) { d = 0xFF; break; }
            }
            if (d != 0xFF) {
                neigh[i][deg[i]]    = (uint16_t)j;
                diff_arr[i][deg[i]] = (uint8_t)d;
                ++deg[i];
            }
        }
    }
}

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
