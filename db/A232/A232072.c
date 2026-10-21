/*
 * A232072  Number of (n+1)X(4+1) 0..1 arrays with every element equal to some
 *          horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 binary arrays in which each element
 *            equals at least one of its horizontal or diagonal neighbors; the
 *            top‑left element is fixed to 0. The recurrence is implemented
 *            via a row‑by‑row DP over all 5‑bit patterns with an
 *            unmatched‑mask that tracks cells still lacking a matching...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* helper structure for a transition */
struct Trans { uint8_t q; uint8_t v; };

static bool initialized = false;
static struct Trans trans_tbl[1024][32];
static uint8_t trans_cnt[1024];

/* precompute all valid transitions between rows */
static void precompute_transitions(void)
{
    for (int p = 0; p < 32; ++p) {
        for (int u = 0; u < 32; ++u) {
            int idx = p * 32 + u;
            uint8_t cnt = 0;
            for (int q = 0; q < 32; ++q) {
                uint8_t match_up_mask   = 0;
                uint8_t match_down_mask = 0;
                uint8_t row_same_mask   = 0;

                for (int j = 0; j < 5; ++j) {
                    uint8_t pj = (p >> j) & 1;
                    uint8_t qj = (q >> j) & 1;

                    /* matches from new row cell to previous row */
                    if (j > 0   && qj == ((p >> (j - 1)) & 1)) match_up_mask   |= 1 << j;
                    if (j < 4   && qj == ((p >> (j + 1)) & 1)) match_up_mask   |= 1 << j;

                    /* matches from previous row cell to new row */
                    if (j < 4   && pj == ((q >> (j + 1)) & 1)) match_down_mask |= 1 << j;
                    if (j > 0   && pj == ((q >> (j - 1)) & 1)) match_down_mask |= 1 << j;

                    /* horizontal matches in the new row */
                    if (j > 0   && qj == ((q >> (j - 1)) & 1)) row_same_mask   |= 1 << j;
                    if (j < 4   && qj == ((q >> (j + 1)) & 1)) row_same_mask   |= 1 << j;
                }

                uint8_t new_u = u & (~match_down_mask);
                if (new_u != 0)        /* previous row would stay unmatched   */
                    continue;

                uint8_t matched_mask = match_up_mask | row_same_mask;
                uint8_t v_mask = (~matched_mask) & 0x1F;

                trans_tbl[idx][cnt].q = (uint8_t)q;
                trans_tbl[idx][cnt].v = v_mask;
                ++cnt;
            }
            trans_cnt[idx] = cnt;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        precompute_transitions();
        initialized = true;
    }

    /* DP state: dp[row_pattern][unmatched_mask] */
    static seq_t dp[32][32];
    static seq_t nextdp[32][32];

    memset(dp, 0, sizeof(dp));

    /* initialise first row (row 0) with top‑left fixed to 0 */
    for (int p = 0; p < 32; ++p) {
        if ((p & 1) != 0)          /* top‑left must be 0 */
            continue;

        /* unmatched cells after inspecting only horizontal neighbours */
        uint8_t row_same = 0;
        for (int j = 0; j < 5; ++j) {
            uint8_t bitj = (p >> j) & 1;
            if (j > 0   && bitj == ((p >> (j - 1)) & 1)) row_same |= 1 << j;
            if (j < 4   && bitj == ((p >> (j + 1)) & 1)) row_same |= 1 << j;
        }
        uint8_t u0 = (~row_same) & 0x1F;
        dp[p][u0] = (seq_t)1;
    }

    /* perform n transitions (row 1 … row n) */
    for (idx_t step = 1; step <= n; ++step) {
        memset(nextdp, 0, sizeof(nextdp));

        for (int p = 0; p < 32; ++p) {
            for (int u = 0; u < 32; ++u) {
                seq_t cur = dp[p][u];
                if (cur == 0)
                    continue;
                int idx = p * 32 + u;
                uint8_t cnt = trans_cnt[idx];
                const struct Trans *tab = trans_tbl[idx];
                for (uint8_t t = 0; t < cnt; ++t) {
                    uint8_t q = tab[t].q;
                    uint8_t v = tab[t].v;
                    nextdp[q][v] += cur;
                }
            }
        }

        /* swap dp and nextdp */
        for (int i = 0; i < 32; ++i)
            memcpy(dp[i], nextdp[i], 32 * sizeof(seq_t));
    }

    /* sum all states with no unmatched cells in the last row */
    seq_t result = (seq_t)0;
    for (int p = 0; p < 32; ++p)
        result += dp[p][0];

    return result;
}
