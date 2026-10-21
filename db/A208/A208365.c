/*
 * A208365  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208365: number of n×4 0..1 arrays avoiding
 *  horizontally the patterns 000 and 001,
 *  and vertically the patterns 001 and 010.
 *
 * The algorithm uses dynamic programming on pairs of consecutive rows.
 * There are 10 admissible 4‑bit rows (those that contain no 000 or 001
 * horizontally). For each triple of rows we pre‑compute whether the
 * vertical constraint is satisfied.  The DP state for length L ≥ 2 is the
 * number of arrays whose last two rows are a particular pair (a,b).
 * Transition:  dp_next[b,c] += dp_cur[a,b] if the triple (a,b,c) is
 * allowed.  Each step recomputes the total for the new length, storing
 * the result in a memoisation cache.  Calls are O(n) for new n and work
 * in any order.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* external integer types supplied by the build */
extern seq_t a(idx_t);   /* forward declaration required by the test harness */

/* Admissible 4‑bit rows (decimal) */
#define NROWS 10
static const unsigned int rows[NROWS] = {
    4, 5, 6, 7, 10, 11, 12, 13, 14, 15
};

/* allowed_next[p][c]  (p = a*NROWS + b).  Value 0/1. */
static unsigned char allowed_next[NROWS * NROWS * NROWS] = {0};

/* Memoisation cache for a(n).  1‑based, cache[0] unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* number of entries allocated */

/* DP arrays for the current and next state.  Size NROWS*NROWS. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static size_t cache_len = 0;     /* largest n computed so far (≥2) */

/* Initialise tables and cache for the first computed values. */
static void init(void)
{
    /* Pre‑compute allowed_next. */
    for (unsigned int a = 0; a < NROWS; ++a) {
        unsigned int ra = rows[a];
        for (unsigned int b = 0; b < NROWS; ++b) {
            unsigned int rb = rows[b];
            unsigned int p = a * NROWS + b;
            for (unsigned int c = 0; c < NROWS; ++c) {
                unsigned int rc = rows[c];
                int ok = 1;
                for (unsigned int j = 0; j < 4; ++j) {
                    int da = (ra >> j) & 1;
                    int db = (rb >> j) & 1;
                    int dc = (rc >> j) & 1;
                    if (da == 0 && db == 0 && dc == 1) { ok = 0; break; } /* 001 */
                    if (da == 0 && db == 1 && dc == 0) { ok = 0; break; } /* 010 */
                }
                allowed_next[(p * NROWS) + c] = ok;
            }
        }
    }

    /* Allocate cache for indices 1 and 2. */
    cache_cap = 3; /* indices 0,1,2 */
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) abort();
    cache[0] = 0;
    cache[1] = (seq_t)10;   /* a(1) */
    cache[2] = (seq_t)100;  /* a(2) */

    /* Allocate DP arrays. */
    dp_cur = (seq_t *)malloc(sizeof(seq_t) * NROWS * NROWS);
    dp_next = (seq_t *)malloc(sizeof(seq_t) * NROWS * NROWS);
    if (!dp_cur || !dp_next) abort();

    /* For length 2, all pairs of admissible rows are possible once. */
    for (size_t i = 0; i < (size_t)NROWS * NROWS; ++i) {
        dp_cur[i] = (seq_t)1;
        dp_next[i] = (seq_t)0;
    }

    cache_len = 2;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    if (!cache) init();   /* ensure tables are ready */

    /* Return cached value if already computed. */
    if ((size_t)n < cache_cap) return cache[n];

    /* Grow cache to accommodate n. */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) abort();
    cache = new_cache;
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    cache_cap = new_cap;

    /* Extend DP from current cache_len up to n. */
    for (size_t len = cache_len + 1; len <= n; ++len) {
        /* Zero dp_next. */
        for (size_t i = 0; i < (size_t)NROWS * NROWS; ++i) dp_next[i] = (seq_t)0;

        /* Transition from dp_cur to dp_next. */
        for (unsigned int a = 0; a < NROWS; ++a) {
            for (unsigned int b = 0; b < NROWS; ++b) {
                size_t p = (size_t)a * NROWS + b;
                seq_t val = dp_cur[p];
                if (!val) continue;
                for (unsigned int c = 0; c < NROWS; ++c) {
                    if (allowed_next[(p * NROWS) + c]) {
                        size_t q = (size_t)b * NROWS + c;
                        dp_next[q] += val;
                    }
                }
            }
        }

        /* Sum total for this length. */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < (size_t)NROWS * NROWS; ++i) total += dp_next[i];
        cache[len] = total;

        /* Swap dp_cur and dp_next for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cache_len = n;
    return cache[n];
}
