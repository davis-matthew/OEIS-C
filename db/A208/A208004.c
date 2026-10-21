/*
 * A208004  Number of nX5 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays that avoid the forbidden
 *            patterns 0 0 1 and 1 0 0 horizontally, 0 1 1 and 1 1 0
 *            vertically. a(1) = 14, a(2) = 196, etc
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming over the last two rows.
   The number of possible rows is 32 (2^5).  A state is encoded as
   (previous_row << 5 | current_row).  For each state we pre‑compute
   which next rows are allowed, and perform a DP transition for every
   additional row.  The results are memoised in a static cache so
   calls in any order are supported. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------------
   Transition tables
   ------------------------------------------------------------------------- */

static bool transitions_initialized = false;
static int  next_count[1024];                    /* number of transitions for each state */
static int  next_states[1024][32];               /* target state of each transition */
static bool allowed_rows[32];                   /* rows that satisfy horizontal constraints */
static int  allowed_count = 0;                   /* number of allowed rows */

/* --------------------------------------------------------------------------
   Cache for the sequence values
   ------------------------------------------------------------------------- */

static seq_t *cache      = NULL;    /* cache of sequence values   */
static size_t cache_cap  = 0;        /* allocated capacity         */
static idx_t  cache_len  = 0;        /* terms filled (0‑based)     */

/* --------------------------------------------------------------------------
   DP arrays for the current and next row
   ------------------------------------------------------------------------- */

static seq_t dp_cur[1024];          /* DP for the current set of last two rows */
static seq_t dp_next[1024];        /* DP for the next set of last two rows */
static bool   dp_initialized = false;/* DP initialisation flag */

/* --------------------------------------------------------------------------
   Helper: pre‑compute allowed rows and transition tables.
   ------------------------------------------------------------------------- */
static void
init_transitions(void)
{
    /* Horizontal restrictions: 001 and 100 are forbidden. */
    for (int r = 0; r < 32; ++r) {
        bool ok = true;
        for (int c = 0; c < 3; ++c) {
            int d0 = (r >> c)        & 1;
            int d1 = (r >> (c + 1))  & 1;
            int d2 = (r >> (c + 2))  & 1;
            if ((d0 == 0 && d1 == 0 && d2 == 1) ||   /* 001 */
                (d0 == 1 && d1 == 0 && d2 == 0)) {   /* 100 */
                ok = false;
                break;
            }
        }
        allowed_rows[r] = ok;
        if (ok) ++allowed_count;
    }

    /* Build vertical transition lists. */
    for (int s = 0; s < 1024; ++s) {
        next_count[s] = 0;
        int r1 = (s >> 5) & 31;  /* row i-2 */
        int r2 = s & 31;         /* row i-1 */
        if (!allowed_rows[r1] || !allowed_rows[r2]) continue;
        for (int r3 = 0; r3 < 32; ++r3) {
            if (!allowed_rows[r3]) continue;
            bool ok = true;
            for (int c = 0; c < 5; ++c) {
                int b1 = (r1 >> c) & 1;
                int b2 = (r2 >> c) & 1;
                int b3 = (r3 >> c) & 1;
                /* vertical forbidden patterns: 0 1 1 and 1 1 0 */
                if ((b1 == 0 && b2 == 1 && b3 == 1) ||
                    (b1 == 1 && b2 == 1 && b3 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            int ns = ((s & 31) << 5) | r3;   /* new state: (r2, r3) */
            next_states[s][next_count[s]++] = ns;
        }
    }

    transitions_initialized = true;
}

/* --------------------------------------------------------------------------
   Initialise DP for the situation after two rows.
   ------------------------------------------------------------------------- */
static void
init_dp(void)
{
    /* dp_cur starts at zero; we fill it with 1 for every allowed pair. */
    for (int r1 = 0; r1 < 32; ++r1) {
        if (!allowed_rows[r1]) continue;
        for (int r2 = 0; r2 < 32; ++r2) {
            if (!allowed_rows[r2]) continue;
            int s = (r1 << 5) | r2;
            dp_cur[s] = (seq_t)1;
        }
    }
}

/* --------------------------------------------------------------------------
   Extend the cache up to a desired index n (n >= 3).
   ------------------------------------------------------------------------- */
static void
extend_cache_to(idx_t n)
{
    /* Ensure cache capacity. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1); /* out of memory */
        if (cache_len > 0) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Grow the sequence term by term. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        /* Zero dp_next. */
        memset(dp_next, 0, sizeof(dp_next));

        /* Transition from every existing state. */
        for (int s = 0; s < 1024; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == 0) continue;
            int cnt = next_count[s];
            for (int i = 0; i < cnt; ++i) {
                int ns = next_states[s][i];
                dp_next[ns] += ways;
            }
        }

        /* Swap dp arrays for next iteration. */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));

        /* Compute total for this n. */
        seq_t total = 0;
        for (int i = 0; i < 1024; ++i) total += dp_cur[i];

        /* Store in cache. */
        cache[cache_len] = total;
        ++cache_len;
    }
}

/* --------------------------------------------------------------------------
   Public function
   ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!transitions_initialized) init_transitions();

    /* Handle the first two terms explicitly. */
    if (n == 1) return (seq_t)allowed_count;
    if (n == 2) return (seq_t)allowed_count * (seq_t)allowed_count;

    /* On first use initialise the DP and the first two cache entries. */
    if (!dp_initialized) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)allowed_count;
        cache[1] = (seq_t)allowed_count * (seq_t)allowed_count;
        cache_len = 2;
        init_dp();
        dp_initialized = true;
    }

    /* Generate values up to the requested index. */
    if (cache_len < n) extend_cache_to(n);

    return cache[n - 1];
}
