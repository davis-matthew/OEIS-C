/*
 * A207665  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   triple_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A207665: Number of 7×n binary arrays avoiding
 *      • horizontally 000 or 111 (any row, any 3 consecutive columns)
 *      • vertically 001 or 100 (any column, any 3 consecutive rows)
 *
 * The algorithm uses a transfer‑matrix dynamic programming:
 *  - Pre–generate all 7‑bit column patterns that satisfy the vertical
 *    constraint (35 patterns).
 *  - For each ordered pair of such columns (c1,c2) pre–compute the list
 *    of columns c3 that can follow without creating a horizontal
 *    illegal triple.
 *  - For n=1, a(n)=35.  For n=2, a(n)=35²=1225.
 *  - For n≥3 the DP iterates over columns; the state is the last
 *    two columns.  Transition from state (c1,c2) to (c2,c3) is allowed
 *    iff (c1,c2,c3) satisfies the horizontal rule.
 *
 * Complexity:   O(n · 35² · avg_transitions) ≈ O(n · 6000),
 *               well below the 2 s limit for n≤10 000.
 * Memory:       2 · (35²) seq_t  + 35² · 35 bytes for transition lists.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/triple_ok.h"

#define NUM_COLS 35          /* number of column types satisfying vertical rule */

/* ----- Precomputation structures ----- */
static uint8_t col_vals[NUM_COLS];          /* 7‑bit vertical‑legal columns */
static uint8_t compat[NUM_COLS * NUM_COLS][NUM_COLS]; /* list of next columns */
static unsigned char compat_cnt[NUM_COLS * NUM_COLS];  /* how many next columns */
static bool initialized = false;

/* Utility: test vertical constraint on a 7‑bit column */
static bool col_ok(uint8_t col) {
    for (int i = 0; i < 5; ++i) {          /* 7 rows => 5 triples */
        uint8_t a = (col >> i) & 1U;
        uint8_t b = (col >> (i + 1)) & 1U;
        uint8_t c = (col >> (i + 2)) & 1U;
        if ((a == 0 && b == 0 && c == 1) ||
            (a == 1 && b == 0 && c == 0))
            return false;
    }
    return true;
}

/* Initializer executed once */
static void init(void) {
    /* 1. Generate all vertical‑legal columns */
    unsigned int idx = 0;
    for (uint8_t col = 0; col < 128; ++col) {
        if (col_ok(col))
            col_vals[idx++] = col;
    }
    /* idx must be NUM_COLS (35) */
    /* 2. Precompute compatibility lists */
    for (unsigned int i = 0; i < NUM_COLS; ++i) {
        for (unsigned int j = 0; j < NUM_COLS; ++j) {
            unsigned int k_idx = i * NUM_COLS + j;
            unsigned char cnt = 0;
            uint8_t c1 = col_vals[i], c2 = col_vals[j];
            for (unsigned int k = 0; k < NUM_COLS; ++k) {
                uint8_t c3 = col_vals[k];
                if (triple_ok(c1, c2, c3))
                    compat[k_idx][cnt++] = (uint8_t)k;
            }
            compat_cnt[k_idx] = cnt;
        }
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0)                    /* undefined below offset, return 0 */
        return 0;

    if (!initialized)
        init();

    /* handle trivial cases directly */
    if (n == 1) return 35;         /* NUM_COLS */
    if (n == 2) return 1225;       /* NUM_COLS * NUM_COLS */

    /* DP arrays (state size = NUM_COLS^2 = 1225) */
    static seq_t dp0[NUM_COLS * NUM_COLS];
    static seq_t dp1[NUM_COLS * NUM_COLS];
    seq_t *dp_cur = dp0, *dp_next = dp1;

    /* initialise for n=2 : each pair of columns has weight 1 */
    for (size_t i = 0; i < NUM_COLS * NUM_COLS; ++i)
        dp_cur[i] = 1;

    /* iterate columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next dp array */
        for (size_t i = 0; i < NUM_COLS * NUM_COLS; ++i)
            dp_next[i] = 0;

        unsigned int state = 0;
        for (unsigned int c1 = 0; c1 < NUM_COLS; ++c1) {
            for (unsigned int c2 = 0; c2 < NUM_COLS; ++c2, ++state) {
                seq_t val = dp_cur[state];
                if (val == 0) continue;
                unsigned char cnt = compat_cnt[state];
                uint8_t *list = compat[state];
                for (unsigned char t = 0; t < cnt; ++t) {
                    unsigned int c3 = list[t];
                    unsigned int new_state = c2 * NUM_COLS + c3;
                    dp_next[new_state] += val;
                }
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all states to get total number of arrays for length n */
    seq_t total = 0;
    for (size_t i = 0; i < NUM_COLS * NUM_COLS; ++i)
        total += dp_cur[i];
    return total;
}
