/*
 * A189622  Number of 7Xn binary arrays without the pattern 0 1 0 diagonally,
 *          antidiagonally or horizontally.
 * https://oeis.org/A189622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   transition_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over the states of two consecutive
 * columns (each 7 bits).  For every column extension we
 * forbid the pattern 0 1 0 horizontally, diagonally (down‑right)
 * or anti‑diagonally (down‑left).  The number of valid 7×n
 * arrays is accumulated in a table and memoised for repeated
 * calls. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/transition_allowed.h"

/* Number of possible columns: 2^7 */
#define COL_BITS   7
#define COL_MAX    (1U << COL_BITS)
#define STATE_COUNT (COL_MAX * COL_MAX)

/* Transition graph information */
static uint32_t *neighbor_start = NULL;    /* start index for each state */
static uint16_t *neighbors_all  = NULL;    /* packed list of successor states */

/* DP vectors */
static seq_t *dp_prev = NULL;              /* counts for current state */
static seq_t *dp_next = NULL;

/* Memoisation table for a(n) */
static seq_t *memo          = NULL;
static size_t memo_capacity = 0;
static size_t memo_size     = 0;
static idx_t  computed_up_to  = 0;   /* maximum n already computed */

/* ------------------------------------------------------------------------ */
static void init(void)
{
    if (neighbor_start != NULL)
        return;                    /* already initialised */

    /* ---- Build transition graph ---- */
    uint16_t *state_out_count = (uint16_t *)malloc(STATE_COUNT * sizeof(uint16_t));
    if (!state_out_count) exit(EXIT_FAILURE);

    /* First pass: count outgoing edges for each state */
    for (uint32_t i = 0; i < STATE_COUNT; ++i) {
        int col0 = i >> COL_BITS;
        int col1 = i & (COL_MAX - 1);
        uint16_t c = 0;
        for (int n = 0; n < COL_MAX; ++n)
            if (transition_allowed(col0, col1, n))
                ++c;
        state_out_count[i] = c;
    }

    /* Prefix sums to determine start indices */
    neighbor_start = (uint32_t *)malloc((STATE_COUNT + 1) * sizeof(uint32_t));
    if (!neighbor_start) exit(EXIT_FAILURE);
    neighbor_start[0] = 0;
    for (uint32_t i = 0; i < STATE_COUNT; ++i)
        neighbor_start[i + 1] = neighbor_start[i] + state_out_count[i];

    uint32_t total_edges = neighbor_start[STATE_COUNT];
    neighbors_all = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    if (!neighbors_all) exit(EXIT_FAILURE);

    /* Second pass: fill neighbor list */
    for (uint32_t i = 0; i < STATE_COUNT; ++i) {
        int col0 = i >> COL_BITS;
        int col1 = i & (COL_MAX - 1);
        uint32_t pos = neighbor_start[i];
        for (int n = 0; n < COL_MAX; ++n) {
            if (transition_allowed(col0, col1, n))
                neighbors_all[pos++] = (uint16_t)((col1 << COL_BITS) | n);
        }
    }

    free(state_out_count);

    /* ---- Allocate DP vectors ---- */
    dp_prev  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    dp_next  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    if (!dp_prev || !dp_next) exit(EXIT_FAILURE);

    for (uint32_t i = 0; i < STATE_COUNT; ++i) {
        dp_prev[i] = (seq_t)1;   /* all pairs of first two columns allowed */
        dp_next[i] = (seq_t)0;
    }

    /* ---- Memoisation table ---- */
    memo_capacity = 64;
    memo          = (seq_t *)malloc(memo_capacity * sizeof(seq_t));
    if (!memo) exit(EXIT_FAILURE);

    memo_size = 3;               /* indices 0,1,2 used */
    memo[0] = (seq_t)0;          /* unused */
    memo[1] = (seq_t)128;        /* a(1) */
    memo[2] = (seq_t)STATE_COUNT;/* a(2) = 2^14 = 16384 */

    computed_up_to = 2;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init(); /* ensure tables are ready */

    /* Return cached value */
    if (n <= computed_up_to)
        return memo[n];

    /* Resize memo table if necessary */
    if ((size_t)n >= memo_capacity) {
        size_t new_cap = memo_capacity;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        memo = tmp;
        memo_capacity = new_cap;
    }

    /* Extend DP from current maximum to n */
    for (idx_t cur = computed_up_to + 1; cur <= n; ++cur) {
        /* Zero dp_next using assignments (works for non‑trivial types) */
        for (uint32_t i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition */
        for (uint32_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_prev[s];
            if (val == (seq_t)0) continue;
            uint32_t start = neighbor_start[s];
            uint32_t end   = neighbor_start[s + 1];
            for (uint32_t idx = start; idx < end; ++idx) {
                uint16_t dest = neighbors_all[idx];
                dp_next[dest] += val;
            }
        }

        /* Sum counts to obtain a(cur) */
        seq_t total = (seq_t)0;
        for (uint32_t i = 0; i < STATE_COUNT; ++i)
            total += dp_next[i];
        memo[cur] = total;

        /* Prepare for next iteration: swap dp vectors */
        seq_t *tmp = dp_prev;
        dp_prev   = dp_next;
        dp_next   = tmp;
    }

    computed_up_to = n;
    return memo[n];
}
