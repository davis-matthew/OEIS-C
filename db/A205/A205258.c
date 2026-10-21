/*
 * A205258  Number of (n+2)X4 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally, and new values 0..1 introduced in row major order.
 * https://oeis.org/A205258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   has_good_line
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A205258 – Number of (n+2)×4 binary arrays whose every 3×3 subblock
    contains a monochromatic line (horizontally, vertically,
    diagonally or antidiagonally).  
    To avoid double counting of complementary arrays the first cell
    (row 0, column 0) is fixed to 0.  The function returns the exact
    value for all n ≥ 1. */

/*  DP state: dp_prev[prev*16+curr]   – number of arrays whose last
    two rows are (prev,curr).  Transition requires that the 3×3
    subblocks formed with the new row satisfy the monochromatic‑line
    property in both column windows (0–2 and 1–3). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_good_line.h"

static uint16_t transition_mask[16][16];
static bool transition_init = false;

/* Build transition masks once.  transition_mask[prev][curr] has bit
   k set if extending (prev,curr) with row k satisfies the
   subblock property. */
static void init_transition(void) {
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            uint16_t mask = 0;
            for (int next = 0; next < 16; ++next) {
                if (has_good_line(prev, curr, next, 0) &&
                    has_good_line(prev, curr, next, 1))
                    mask |= (uint16_t)(1u << next);
            }
            transition_mask[prev][curr] = mask;
        }
    }
    transition_init = true;
}

/* Main routine.  The array is canonically represented by fixing
   the top‑left cell to 0, which removes the duplicate counting of
   complementary arrays. */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!transition_init) init_transition();

    /* DP arrays: 16 × 16 = 256 states. */
    seq_t dp_prev[256];
    seq_t dp_cur[256];

    /* Initialise with all possible first two rows, but require the
       first cell (bit 0 of the first row) to be 0. */
    for (int i = 0; i < 256; ++i) dp_prev[i] = 0;
    for (int prev = 0; prev < 16; ++prev) {
        if ((prev & 1) == 0) {                /* first bit must be 0 */
            for (int curr = 0; curr < 16; ++curr)
                dp_prev[prev * 16 + curr] = 1;
        }
    }

    /* Add n additional rows to reach a total of n + 2 rows. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) dp_cur[i] = 0;
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t val = dp_prev[prev * 16 + curr];
                if (val == 0) continue;
                uint16_t mask = transition_mask[prev][curr];
                if (mask == 0) continue;
                for (int next = 0; next < 16; ++next) {
                    if (mask & (1u << next))
                        dp_cur[curr * 16 + next] += val;
                }
            }
        }
        for (int i = 0; i < 256; ++i) dp_prev[i] = dp_cur[i];
    }

    /* Sum over all final states. */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i) ans += dp_prev[i];
    return ans;
}
