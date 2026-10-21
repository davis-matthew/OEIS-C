/*
 * A218352  Number of n X 6 binary arrays with every 0 a horizontal or
 *          vertical neighbor to some 1.
 * https://oeis.org/A218352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   horiz_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts n×6 binary arrays in which every 0 has at least one
 * horizontally or vertically adjacent 1.  The algorithm is a dynamic
 * programming over rows.  The state is the last row pattern `prev` and
 * the mask `unsat` of columns where a 1 is required in the next row
 * (because the previous row had a 0 with no horizontal neighbour
 * and no 1 above).  Transitions are performed by enumerating
 * supermasks of `unsat`.  Complexity is O(n·64²) with a very small
 * constant factor suitable for n up to 10000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/horiz_mask.h"

static const uint8_t ALL_MASK = 0x3F; /* 6 bits */

/* Pre‑computed tables */
static uint8_t req[64][64];          /* req[a][b] = required mask for next row
                                      *   after rows a then b */
static uint8_t supermask_list[64][64];
static uint8_t supermask_cnt[64];

/* ---------- pre‑compute req and supermask lists ---------- */
static void init_tables(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* compute req[a][b] */
    for (uint8_t a = 0; a < 64; ++a) {
        for (uint8_t b = 0; b < 64; ++b) {
            uint8_t hm = horiz_mask(b);
            uint8_t mask = 0;
            for (int j = 0; j < 6; ++j) {
                if (((b >> j) & 1) == 0) {           /* current cell is 0 */
                    if (((hm >> j) & 1) == 0) {          /* no horizontal 1 */
                        if (((a >> j) & 1) == 0) {          /* no 1 above */
                            mask |= (1 << j);                /* need 1 below */
                        }
                    }
                }
            }
            req[a][b] = mask;
        }
    }

    /* supermask lists */
    for (uint8_t mask = 0; mask < 64; ++mask) {
        uint8_t cnt = 0;
        for (uint8_t s = 0; s < 64; ++s) {
            if ((s & mask) == mask) {
                supermask_list[mask][cnt++] = s;
            }
        }
        supermask_cnt[mask] = cnt;
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;        /* undefined by offset, but safe */

    /* dp[prev][unsat] */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* first row: prevPrev = 0 (virtual row of zeros) */
    for (uint8_t r = 0; r < 64; ++r) {
        uint8_t unsat = req[0][r];
        dp[r][unsat] = (seq_t)1;
    }

    /* process remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = 0;

        for (uint8_t prev = 0; prev < 64; ++prev) {
            for (uint8_t unsat = 0; unsat < 64; ++unsat) {
                seq_t cnt = dp[prev][unsat];
                if (cnt == 0) continue;
                uint8_t* list = supermask_list[unsat];
                uint8_t list_cnt = supermask_cnt[unsat];
                for (uint8_t k = 0; k < list_cnt; ++k) {
                    uint8_t cur = list[k];
                    uint8_t next_unsat = req[prev][cur];
                    ndp[cur][next_unsat] += cnt;
                }
            }
        }
        /* copy back */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final sum: unsatisfied mask must be zero for the last row */
    seq_t ans = 0;
    for (uint8_t prev = 0; prev < 64; ++prev) {
        ans += dp[prev][0];
    }
    return ans;
}
