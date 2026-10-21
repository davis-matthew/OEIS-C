/*
 * A238306  Number of (n+1) X (1+1) 0..3 arrays with the maximum minus the
 *          lower median of every 2 X 2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A238306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays over 0..3 where for every 2×2
 *            subblock the difference (max minus lower median) of the subblock
 *            differs by exactly one from that of its horizontal and vertical
 *            neighbors. This is computed by dynamic programming on the
 *            difference values and the right column of each s...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   absdiff_int, lower_median_of_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/absdiff_int.h"
#include "../utils/lower_median_of_four.h"

/* precompute difference table for all column pairs */
static uint8_t diff_table[16][16];

static void init_diff_table(void) {
    static bool initialized = false;
    if (initialized) return;
    for (int c1 = 0; c1 < 16; ++c1) {
        int top1 = (c1 >> 2) & 3;
        int bot1 =  c1 & 3;
        for (int c2 = 0; c2 < 16; ++c2) {
            int top2 = (c2 >> 2) & 3;
            int bot2 =  c2 & 3;
            int vals[4] = {top1, bot1, top2, bot2};
            int maxv = vals[0];
            for (int i = 1; i < 4; ++i) if (vals[i] > maxv) maxv = vals[i];
            lower_median_of_four(vals);
            int lmed = vals[1];
            diff_table[c1][c2] = (uint8_t)(maxv - lmed);
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1, but safe */
    init_diff_table();                 /* ensure diff_table is ready */

    /* state id = column index (0..15) * 4 + difference (0..3) */
    seq_t dp_prev[64] = {0};
    seq_t dp_curr[64] = {0};

    /* --- initial block (first two columns) --- */
    for (int c2 = 0; c2 < 16; ++c2) {
        for (int c1 = 0; c1 < 16; ++c1) {
            uint8_t d = diff_table[c1][c2];
            int state = (c2 << 2) | d;
            dp_prev[state] = dp_prev[state] + (seq_t)1;
        }
    }

    /* --- subsequent columns --- */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) dp_curr[i] = (seq_t)0;

        for (int id1 = 0; id1 < 64; ++id1) {
            seq_t ways = dp_prev[id1];
            if (!ways) continue;
            int c1 = id1 >> 2;
            int d1 = id1 & 3;
            for (int c2 = 0; c2 < 16; ++c2) {
                uint8_t d2 = diff_table[c1][c2];
                if (absdiff_int(d1, d2) == 1) {
                    int id2 = (c2 << 2) | d2;
                    dp_curr[id2] = dp_curr[id2] + ways;
                }
            }
        }
        /* move to next step */
        for (int i = 0; i < 64; ++i) dp_prev[i] = dp_curr[i];
    }

    /* total count = sum of all end states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) {
        total = total + dp_prev[i];
    }
    return total;
}
