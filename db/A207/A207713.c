/*
 * A207713  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   vert_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over rows with a two‑row state to count  n×4 binary arrays
   avoiding horizontal patterns 000 and 001 and vertical patterns 001 and 100. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/vert_ok.h"

/* ------------------ global tables ------------------ */
static uint8_t horiz_ok[16];               /* 1 if a 4‑bit row is horizontal‑valid */
static uint8_t trans_next[256][16];        /* successor rows for a given state */
static uint8_t trans_count[256];           /* number of successors per state */
static seq_t count_h;                     /* number of horizontal‑valid rows */
static bool init_done = false;

/* ------------------ helpers ------------------ */
static inline bool row_ok(uint8_t row)
{
    /* check both windows of length 3 */
    for (int shift = 0; shift <= 1; ++shift) {
        uint8_t bits = (row >> shift) & 0x7;       /* 3 consecutive bits */
        if (bits == 0x0 || bits == 0x1)            /* 000 or 001 */
            return false;
    }
    return true;
}

/* ------------------ one‑time table init ------------------ */
static void init_tables(void)
{
    if (init_done)
        return;

    /* horizontal validity and total count */
    count_h = 0;
    for (uint8_t row = 0; row < 16; ++row) {
        horiz_ok[row] = row_ok(row) ? 1 : 0;
        if (horiz_ok[row])
            count_h += (seq_t)1;
    }

    /* transition table */
    for (uint16_t state = 0; state < 256; ++state) {
        uint8_t prev2 = state >> 4;
        uint8_t prev1 = state & 0xF;
        uint8_t cnt = 0;
        for (uint8_t next = 0; next < 16; ++next) {
            if (!horiz_ok[prev1] || !horiz_ok[next])
                continue;
            if (!vert_ok(prev2, prev1, next))
                continue;
            trans_next[state][cnt++] = next;
        }
        trans_count[state] = cnt;
    }

    init_done = true;
}

/* ------------------ main sequence function ------------------ */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0)                /* defined only for n ≥ 1 */
        return (seq_t)0;

    if (n == 1)
        return count_h;

    if (n == 2)
        return count_h * count_h;

    /* DP arrays for two‑row states */
    seq_t dp[256]  = {0};
    seq_t ndp[256] = {0};

    /* initial states for first two rows */
    for (uint8_t r1 = 0; r1 < 16; ++r1) {
        if (!horiz_ok[r1])
            continue;
        for (uint8_t r2 = 0; r2 < 16; ++r2) {
            if (!horiz_ok[r2])
                continue;
            uint16_t state = (r1 << 4) | r2;
            dp[state] += (seq_t)1;
        }
    }

    /* iterate remaining rows */
    for (idx_t row = 3; row <= n; ++row) {
        for (uint16_t s = 0; s < 256; ++s)
            ndp[s] = 0;

        for (uint16_t s = 0; s < 256; ++s) {
            seq_t ways = dp[s];
            if (!ways)
                continue;

            uint8_t prev1 = s & 0xF;
            uint8_t cnt   = trans_count[s];

            for (uint8_t k = 0; k < cnt; ++k) {
                uint8_t next = trans_next[s][k];
                uint16_t ns  = (prev1 << 4) | next;
                ndp[ns] += ways;
            }
        }

        for (uint16_t s = 0; s < 256; ++s)
            dp[s] = ndp[s];
    }

    /* sum all final states */
    seq_t total = 0;
    for (uint16_t s = 0; s < 256; ++s)
        total += dp[s];

    return total;
}
