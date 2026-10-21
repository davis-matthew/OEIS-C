/*
 * A202800  Number of nX6 binary arrays with every one adjacent to another one
 *          horizontally or vertically.
 * https://oeis.org/A202800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with every 1 adjacent to
 *            another 1 horizontally or vertically. Computed by dynamic
 *            programming over rows
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Transition tables (static, built once) */
static uint16_t trans[4096][64];
static uint8_t  trans_cnt[4096];
static bool initialized = false;

static void build_transitions(void) {
    for (uint16_t s = 0; s < 4096; ++s) {
        uint8_t prev_row   = s & 0x3F;
        uint8_t prev_unsat = (s >> 6) & 0x3F;
        /* Illegal state: unsatisfied bits not part of the row */
        if (prev_unsat & (~prev_row)) {
            trans_cnt[s] = 0;
            continue;
        }
        uint8_t cnt = 0;
        for (uint8_t cur_row = 0; cur_row < 64; ++cur_row) {
            /* Each unsatisfied bit of the previous row must be 1 in the current row */
            if (prev_unsat & (~cur_row)) continue;
            /* Compute unsatisfied bits in the current row */
            uint8_t unsat_cur = 0;
            for (int j = 0; j < 6; ++j) {
                if ((cur_row >> j) & 1U) {
                    bool sat = false;
                    /* left neighbor */
                    if (j > 0 && ((cur_row >> (j - 1)) & 1U)) sat = true;
                    /* right neighbor */
                    if (!sat && j < 5 && ((cur_row >> (j + 1)) & 1U)) sat = true;
                    /* up neighbor */
                    if (!sat && ((prev_row >> j) & 1U)) sat = true;
                    if (!sat) unsat_cur |= (1U << j);
                }
            }
            uint16_t dest = (uint16_t)cur_row | ((uint16_t)unsat_cur << 6);
            trans[s][cnt++] = dest;
        }
        trans_cnt[s] = cnt;
    }
}

seq_t a(idx_t n) {
    /* Ensure transitions are ready */
    if (!initialized) {
        build_transitions();
        initialized = true;
    }
    if (n == 0) return (seq_t)0;   /* out of range; defined as 0 */

    /* DP arrays for current and next row */
    static seq_t dp[4096];
    static seq_t ndp[4096];
    /* Initialize dp for 0 rows (prev_row=0, unsat=0) */
    for (size_t i = 0; i < 4096; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t row = 1; row <= n; ++row) {
        for (size_t i = 0; i < 4096; ++i) ndp[i] = (seq_t)0;
        for (uint16_t s = 0; s < 4096; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            uint8_t cnt = trans_cnt[s];
            for (uint8_t t = 0; t < cnt; ++t) {
                uint16_t d = trans[s][t];
                ndp[d] += val;
            }
        }
        /* swap dp and ndp */
        for (size_t i = 0; i < 4096; ++i) dp[i] = ndp[i];
    }

    /* Sum states with no unsatisfied bits (unsatMask == 0) */
    seq_t ans = (seq_t)0;
    for (uint16_t prev = 0; prev < 64; ++prev) {
        ans += dp[prev];
    }
    return ans;
}
