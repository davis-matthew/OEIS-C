/*
 * A234109  Number of (n+1)X(3+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 12.
 * https://oeis.org/A234109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A234109 – number of (n+1)×4 arrays 0..4
 *  with every 2×2 subblock having the sum of the absolute values
 *  of all six edge and diagonal differences equal to 12.
 *
 *  The implementation counts the arrays by dynamic programming on all
 *  5⁴ = 625 possible row patterns.  For each row pattern we pre‑compute
 *  the list of compatible next rows (those that satisfy the 2×2‐block
 *  condition in the three column positions).  For a given height n+1 the
 *  number of arrays is the sum of all ways to build a sequence of n+1
 *  rows using these transitions.  The values are memoised so that calls
 *  with arbitrary n are fast and independent of call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
extern void abort(void);

static bool init_done = false;
static uint8_t rows[625][4];
static uint16_t next_states[625 * 625];
static uint32_t next_start[626];
static uint32_t next_count[625];

static seq_t dp_cur[625];
static seq_t dp_next[625];

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements allocated in cache. */
static idx_t cached_n = 0;      /* highest n for which dp_cur holds the
                                 * counts of arrays of height n+1. */

static inline int abs_int(int x) { return x >= 0 ? x : -x; }

/* Check whether two rows are compatible: all three 2×2 blocks have
 * sum of the six differences equal to 12. */
static bool rows_compatible(const uint8_t *r1, const uint8_t *r2)
{
    for (int k = 0; k < 3; ++k) {
        int a = r1[k];
        int b = r1[k + 1];
        int c = r2[k];
        int d = r2[k + 1];
        int sum = abs_int(a - b) + abs_int(c - d) + abs_int(a - c) +
                  abs_int(b - d) + abs_int(a - d) + abs_int(b - c);
        if (sum != 12) return false;
    }
    return true;
}

/* Initialise all 625 row patterns and the transition lists. */
static void init_state_space(void)
{
    /* Enumerate all row patterns (base‑5). */
    for (int r = 0; r < 625; ++r) {
        int x = r;
        for (int pos = 3; pos >= 0; --pos) {
            rows[r][pos] = (uint8_t)(x % 5);
            x /= 5;
        }
    }

    /* Build transition lists. */
    uint32_t edge_cnt = 0;
    for (int i = 0; i < 625; ++i) {
        next_start[i] = edge_cnt;
        for (int j = 0; j < 625; ++j) {
            if (rows_compatible(rows[i], rows[j])) {
                next_states[edge_cnt++] = (uint16_t)j;
            }
        }
    }
    next_start[625] = edge_cnt;
    for (int i = 0; i < 625; ++i) next_count[i] = next_start[i + 1] - next_start[i];

    /* Initialise DP for zero steps: one way for each starting row. */
    for (int i = 0; i < 625; ++i) dp_cur[i] = 1;
    cached_n = 0;

    init_done = true;
}

/* Ensure the cache holds sequence values up to n. */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    if (!init_done) init_state_space();

    /* Grow cache array if needed. */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) abort();
        /* Initialise new entries to zero. */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Iterate DP steps until we reach requested n. */
    while (cached_n < n) {
        /* Compute dp_next from dp_cur. */
        for (int i = 0; i < 625; ++i) dp_next[i] = 0;

        for (int i = 0; i < 625; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            uint32_t start = next_start[i];
            uint32_t cnt   = next_count[i];
            for (uint32_t k = 0; k < cnt; ++k) {
                int j = next_states[start + k];
                dp_next[j] += val;
            }
        }

        /* Swap buffers. */
        for (int i = 0; i < 625; ++i) dp_cur[i] = dp_next[i];

        /* Sum to get a(cached_n+1). */
        seq_t total = 0;
        for (int i = 0; i < 625; ++i) total += dp_cur[i];
        cache[++cached_n] = total;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
