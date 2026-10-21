/*
 * A238724  Number of nX6 0..2 arrays with no element equal to the sum modulo
 *          3 of elements to its left or elements above it.
 * https://oeis.org/A238724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   digits_to_index, index_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A238724: number of n×6 arrays with entries 0..2
   in which no element equals the sum modulo 3 of elements to its
   left or of the elements above it. */
/* The algorithm uses dynamic programming over the vector of
   column‑sum residues modulo 3.  Each state is a 6‑digit base‑3
   number (3^6 = 729 states).  For every state we precompute the
   list of possible next states and how many rows realise each
   transition.  Then the DP iterates over rows, accumulating all
   possible configurations.  The values are stored in seq_t; the
   routine is fully memoized to be efficient for repeated calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* memset, memcpy */
#include <stdlib.h>   /* calloc, realloc */
#include "../utils/digits_to_index.h"
#include "../utils/index_to_digits.h"

/* Transition tables */
#define STATES   729          /* 3^6 possible column‑sum states */
#define MAX_TRANS 64          /* ≤ 2^6 rows per state */

static int    trans_target[STATES][MAX_TRANS];
static unsigned char trans_count[STATES][MAX_TRANS];
static unsigned char trans_len[STATES];
static bool    trans_ready = false;

/* Recursive DFS that builds all admissible rows for a given
   starting column‑sum vector.  'origin' is the source state index.
   'cur' holds the current column sums modulo 3. */
static void dfs_build(int col, int left_sum, int cur[6], int origin) {
    if (col == 6) {
        int dst = digits_to_index(cur);
        /* Record transition (origin -> dst).  Count duplicates. */
        unsigned char len = trans_len[origin];
        for (unsigned char k = 0; k < len; ++k) {
            if (trans_target[origin][k] == dst) {
                trans_count[origin][k] += 1;
                return;
            }
        }
        trans_target[origin][len] = dst;
        trans_count[origin][len] = 1;
        trans_len[origin] = (unsigned char)(len + 1);
        return;
    }

    int forb_left = left_sum;    /* sum of left elements in current row (mod 3) */
    int forb_above = cur[col];   /* cumulative sum above this column (mod 3) */

    for (int v = 0; v < 3; ++v) {
        if (v == forb_left || v == forb_above) continue;    /* forbidden value */
        int saved = cur[col];
        cur[col] = (cur[col] + v) % 3;
        dfs_build(col + 1, (left_sum + v) % 3, cur, origin);
        cur[col] = saved;               /* backtrack */
    }
}

/* Build the transition tables once. */
static void init_transitions(void) {
    if (trans_ready) return;
    for (int idx = 0; idx < STATES; ++idx) {
        int init[6];
        index_to_digits(idx, init);
        int cur[6];
        memcpy(cur, init, sizeof(cur));
        dfs_build(0, 0, cur, idx);
    }
    trans_ready = true;
}

/* Main sequence routine. */
seq_t a(idx_t n) {
    static seq_t *dp_cur  = NULL;  /* dp for current number of rows */
    static seq_t *dp_nxt  = NULL;  /* dp for next number of rows */
    static size_t dp_alloc = 0;
    static seq_t *cache    = NULL;  /* memoized a(n) values, 1‑based */
    static size_t cache_cap = 0;
    static idx_t cache_len  = 0;   /* highest n cached */

    /* Initialise transition tables. */
    init_transitions();

    /* Allocate DP arrays on first call. */
    if (!dp_cur) {
        dp_alloc = STATES;
        dp_cur   = (seq_t *)calloc(dp_alloc, sizeof(seq_t));
        dp_nxt   = (seq_t *)calloc(dp_alloc, sizeof(seq_t));
        dp_cur[0] = 1;  /* empty configuration: all column sums are 0 */
    }

    /* Allocate memoization cache on first call. */
    if (!cache) {
        cache_cap = 2;                 /* indices 0 and 1 (unused 0) */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 0;                  /* unused */
    }

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n];

    /* Extend cache size if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Preserve existing values; new entries are implicitly zero. */
        cache_cap = new_cap;
    }

    /* Compute values incrementally up to n. */
    for (idx_t rows = cache_len + 1; rows <= n; ++rows) {
        memset(dp_nxt, 0, dp_alloc * sizeof(seq_t));

        /* Transition from all source states. */
        for (int src = 0; src < STATES; ++src) {
            seq_t ways = dp_cur[src];
            if (!ways) continue;
            unsigned char len = trans_len[src];
            for (unsigned char k = 0; k < len; ++k) {
                int dst = trans_target[src][k];
                unsigned char cnt = trans_count[src][k];
                dp_nxt[dst] = dp_nxt[dst] + ways * (seq_t)cnt;
            }
        }

        /* Sum over all destination states gives a(rows). */
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s) total = total + dp_nxt[s];
        cache[rows] = total;

        /* Prepare for next iteration. */
        seq_t *tmp  = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    cache_len = n;
    return cache[n];
}
