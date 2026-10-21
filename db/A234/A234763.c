/*
 * A234763  Number of (n+1)X(2+1) 0..3 arrays with 2X2 edge jumps all no more
 *          than +1 in one of the clockwise or counterclockwise directions but
 *          not both.
 * https://oeis.org/A234763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234763: Number of (n+1)×3 0..3 arrays with the 2×2 edge‑jump condition.
   Exactly one of the clockwise or counter‑clockwise 2×2 edge‑jump sums
   is ≤ +1, but not both.  Offset 1.  The sequence is given for n ≥ 1. */

/*  The recurrence is not directly provided; we use dynamic programming over
    the 64 possible 3‑column rows of the array.  For each row pattern we
    pre‑compute which next row patterns are allowed.  The DP counts the
    number of ways to extend from the top row to the bottom over n+1 rows. */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Helper allocation functions for the arbitrary‑precision `seq_t` type. */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Transition table: from each 3‑column pattern i (0..63) to the set of
   allowed patterns j.  The patterns are encoded in base‑4:
   bits 6‑5 – left column, bits 4‑3 – middle, bits 2‑1 – right. */
static int  next_count[64];
static int  next_list[64][64];   /* maximum 64 successors per state */

/* Pre‑compute validity of transitions between rows. */
static void precompute_transitions(void)
{
    static bool done = false;
    if (done) return;
    for (int i = 0; i < 64; ++i) {
        int vi0 = i >> 4;
        int vi1 = (i >> 2) & 3;
        int vi2 = i & 3;
        int cnt = 0;
        for (int j = 0; j < 64; ++j) {
            int vj0 = j >> 4;
            int vj1 = (j >> 2) & 3;
            int vj2 = j & 3;

            /* First 2×2 window: columns 0‑1 */
            int a = vi0, b = vi1, c = vj0, d = vj1;
            bool cw1  = (b - a <= 1) && (d - b <= 1) &&
                        (c - d <= 1) && (a - c <= 1);
            bool ccw1 = (c - a <= 1) && (d - c <= 1) &&
                        (b - d <= 1) && (a - b <= 1);
            if (!(cw1 ^ ccw1)) continue;

            /* Second 2×2 window: columns 1‑2 */
            a = vi1; b = vi2; c = vj1; d = vj2;
            bool cw2  = (b - a <= 1) && (d - b <= 1) &&
                        (c - d <= 1) && (a - c <= 1);
            bool ccw2 = (c - a <= 1) && (d - c <= 1) &&
                        (b - d <= 1) && (a - b <= 1);
            if (!(cw2 ^ ccw2)) continue;

            next_list[i][cnt++] = j;
        }
        next_count[i] = cnt;
    }
    done = true;
}

/* DP arrays and caching state */
static seq_t dp1[64];
static seq_t dp2[64];
static seq_t *current = dp1;
static seq_t *next    = dp2;

/* Cache of previously computed values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated size of cache */
static idx_t  max_cached_n = 0; /* largest n for which cache[n] is valid */
static idx_t  cache_rows   = 1; /* number of rows processed (rows = n+1) */

/* Ensure the cache can hold index n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function; external linkage */
seq_t a(idx_t n)
{
    /* Lazily initialise data structures on first call */
    static bool initialized = false;
    if (!initialized) {
        precompute_transitions();

        /* Start DP with one row: each of the 64 patterns is possible once */
        for (int i = 0; i < 64; ++i) current[i] = (seq_t)1;

        /* Initialise cache */
        cache = seq_alloc_new_array(1);
        cache[0] = 0;                 /* unused placeholder  */
        cache_cap = 1;
        max_cached_n = 0;
        cache_rows = 1;                /* one row processed */
        initialized = true;
    }

    if (n <= max_cached_n) return cache[n];

    /* Grow cache if necessary */
    ensure_cache_capacity(n);

    /* Extend DP to cover n+1 rows (target_rows = n+1) */
    idx_t target_rows = n + 1;
    for (idx_t r = cache_rows + 1; r <= target_rows; ++r) {
        /* Compute next DP row */
        for (int i = 0; i < 64; ++i) next[i] = 0;

        for (int i = 0; i < 64; ++i) {
            seq_t val = current[i];
            if (!val) continue;
            int  cnt = next_count[i];
            const int *nbr = next_list[i];
            for (int k = 0; k < cnt; ++k) {
                int j = nbr[k];
                next[j] += val;
            }
        }

        /* Swap currents */
        seq_t *tmp = current;
        current = next;
        next    = tmp;

        /* Sum counts for this number of rows (a(r-1)) */
        seq_t sum = 0;
        for (int i = 0; i < 64; ++i) sum += current[i];
        cache[r - 1] = sum;              /* a(r-1) stored at index r-1 */
    }

    cache_rows   = target_rows;
    max_cached_n = n;
    return cache[n];
}
