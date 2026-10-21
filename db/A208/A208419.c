/*
 * A208419  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding the * forbidden 0 0
 *            0 and 0 0 1 horizontal patterns and * 0 0 0 and 0 1 1 vertical
 *            patterns. * * The sequence is computed by dynamic programming on
 *            the last two rows. * For every allowed row (42 possibilities) we
 *            precompute, for every * pair of previous...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 31, u128 n <= 70,
 *            big any n
 * Helpers:   compatible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compatible.h"

/* ------------------------------------------------------------ */
/* Helper: Check whether a 7‑bit row avoids 000 and 001 horizontally   */
static bool is_allowed_row(int v)
{
    for (int j = 0; j <= 4; ++j) {            /* examine 3‑bit windows */
        int pat = (v >> j) & 7;               /* 3 bits starting at j */
        if (pat == 0 || pat == 1)             /* 000 or 001 forbidden */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Global precomputed tables                                          */
#define ALLOWED_SIZE 42
#define STATE_SIZE   (ALLOWED_SIZE * ALLOWED_SIZE)

static int ROW_INDEX[128];                 /* value -> index in ALLOWED_ROWS (-1 if not allowed) */
static int ALLOWED_ROWS[ALLOWED_SIZE];     /* list of all allowed rows */
static uint64_t ADJ_MASKS[STATE_SIZE];     /* mask[j] : bits of rows allowed after pair (i,j) */
static bool INIT_DONE = false;

/* Fill tables on first call */
static void init_tables(void)
{
    if (INIT_DONE) return;

    /* Build list of allowed rows and mapping */
    int count = 0;
    for (int v = 0; v < 128; ++v) {
        if (is_allowed_row(v)) {
            ROW_INDEX[v] = count;
            ALLOWED_ROWS[count] = v;
            ++count;
        } else {
            ROW_INDEX[v] = -1;
        }
    }

    /* Build adjacency masks */
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {
            uint64_t mask = 0ULL;
            for (int k = 0; k < count; ++k) {
                if (compatible(ALLOWED_ROWS[i], ALLOWED_ROWS[j], ALLOWED_ROWS[k])) {
                    mask |= ((uint64_t)1 << k);
                }
            }
            ADJ_MASKS[i * count + j] = mask;
        }
    }

    INIT_DONE = true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;          /* not used, offset 1 */
    if (n == 1) return (seq_t)42;  /* single row: any allowed 7‑bit pattern */
    if (n == 2) return (seq_t)1764;/* two rows: all pairs of allowed rows */

    /* DP arrays – static to avoid repeated allocation */
    static seq_t dp_cur[STATE_SIZE];
    static seq_t dp_next[STATE_SIZE];

    /* Base: all pairs of rows are possible for height = 2 */
    for (int idx = 0; idx < STATE_SIZE; ++idx)
        dp_cur[idx] = 1;

    /* Iterate from height 3 up to n */
    for (idx_t height = 3; height <= n; ++height) {
        /* Zero next state */
        for (int idx = 0; idx < STATE_SIZE; ++idx)
            dp_next[idx] = 0;

        /* Transition */
        for (int a_idx = 0; a_idx < ALLOWED_SIZE; ++a_idx) {
            for (int b_idx = 0; b_idx < ALLOWED_SIZE; ++b_idx) {
                seq_t val = dp_cur[a_idx * ALLOWED_SIZE + b_idx];
                if (val == 0) continue;

                uint64_t mask = ADJ_MASKS[a_idx * ALLOWED_SIZE + b_idx];
                /* iterate over set bits in mask */
                while (mask) {
                    int c_idx = __builtin_ctzll(mask);
                    dp_next[b_idx * ALLOWED_SIZE + c_idx] += val;
                    mask &= mask - 1;
                }
            }
        }

        /* Prepare for next loop */
        for (int idx = 0; idx < STATE_SIZE; ++idx)
            dp_cur[idx] = dp_next[idx];
    }

    /* Sum all states to obtain a(n) */
    seq_t result = 0;
    for (int idx = 0; idx < STATE_SIZE; ++idx)
        result += dp_cur[idx];

    return result;
}
