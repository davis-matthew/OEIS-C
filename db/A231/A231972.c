/*
 * A231972  Number of (n+1)X(3+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal and vertical neighbors equal to one.
 * https://oeis.org/A231972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   valid_middle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231972: Number of (n+1)×4 binary arrays with no element
   having a strict majority of its horizontal and vertical neighbors equal
   to one.
   Computed by dynamic programming over pairs of consecutive rows. */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/valid_middle.h"

/*--------------------------------------------------------------------
 *  Helper functions that test the condition for a single row.
 *  The cell itself is NOT counted as a neighbor.
 *--------------------------------------------------------------------*/

static bool valid_top(uint8_t top, uint8_t down)
{
    for (int j = 0; j < 4; ++j) {
        int ones = 0;
        if (j > 0)     ones += (top >> (j - 1)) & 1;
        if (j < 3)     ones += (top >> (j + 1)) & 1;
        ones += (down >> j) & 1;
        int neigh = (j > 0) + (j < 3) + 1;          /* left/right/down */
        if (ones > neigh / 2) return false;
    }
    return true;
}

static bool valid_bottom(uint8_t up, uint8_t bot)
{
    for (int j = 0; j < 4; ++j) {
        int ones = 0;
        if (j > 0)     ones += (bot >> (j - 1)) & 1;
        if (j < 3)     ones += (bot >> (j + 1)) & 1;
        ones += (up >> j) & 1;
        int neigh = (j > 0) + (j < 3) + 1;          /* left/right/up */
        if (ones > neigh / 2) return false;
    }
    return true;
}

/*--------------------------------------------------------------------
 *  Pre‑computed tables:
 *    top_valid[ up ][ down ]   – whether a top row with the following
 *                                 row is locally valid.
 *    bottom_valid[ up ][ bot ] – whether a bottom row with the
 *                                 preceding row is locally valid.
 *    compat[ up ][ mid ]       – bitmask of next rows that allow
 *                                 (up,mid,down) to be valid.
 *--------------------------------------------------------------------*/

static bool  top_valid[16][16];
static bool  bottom_valid[16][16];
static uint16_t compat[16][16];

static void build_tables(void)
{
    for (int up = 0; up < 16; ++up) {
        for (int cur = 0; cur < 16; ++cur) {
            top_valid[up][cur]     = valid_top((uint8_t)up, (uint8_t)cur);
            bottom_valid[up][cur]  = valid_bottom((uint8_t)up, (uint8_t)cur);

            uint16_t mask = 0;
            for (int down = 0; down < 16; ++down)
                if (valid_middle((uint8_t)up, (uint8_t)cur, (uint8_t)down))
                    mask |= (1U << down);
            compat[up][cur] = mask;
        }
    }
}

/*--------------------------------------------------------------------
 *  Main sequence function
 *--------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1 */
        return (seq_t)0;

    static bool initialized = false;
    if (!initialized) {
        build_tables();
        initialized = true;
    }

    const idx_t rows = n + 1;          /* number of rows in the array */

    static seq_t dp_cur[256];
    static seq_t dp_next[256];
    memset(dp_cur, 0, sizeof(dp_cur));

    /* initial pair of rows: top row (r0) and second row (r1) */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            if (top_valid[r0][r1])
                dp_cur[(r0 << 4) | r1] = (seq_t)1;
        }
    }

    /* iterate for remaining rows (step 2 ... rows-1) */
    for (idx_t step = 2; step < rows; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int state = 0; state < 256; ++state) {
            seq_t cnt = dp_cur[state];
            if (!cnt) continue;
            int up  = state >> 4;
            int cur = state & 15;
            uint16_t mask = compat[up][cur];
            for (int down = 0; down < 16; ++down) {
                if (mask & (1U << down)) {
                    int new_state = (cur << 4) | down;
                    dp_next[new_state] += cnt;
                }
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* final row must satisfy the bottom condition */
    seq_t ans = 0;
    for (int state = 0; state < 256; ++state) {
        seq_t cnt = dp_cur[state];
        if (!cnt) continue;
        int up  = state >> 4;
        int bot = state & 15;
        if (bottom_valid[up][bot])
            ans += cnt;
    }
    return ans;
}
