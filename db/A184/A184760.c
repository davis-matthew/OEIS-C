/*
 * A184760  Half the number of nX8 binary arrays with no 1 having an adjacent
 *          1 both above and to its left.
 * https://oeis.org/A184760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A184760: Half the number of n×8 binary arrays with no 1
    having an adjacent 1 both above and to its left.
    The value a(n) is computed by dynamic programming on the 256
    possible 8‑bit row patterns.  For a previous row pattern p and a
    current row pattern q the transition p→q is allowed iff
    for every column j>0 it is NOT the case that
    q has a 1 in column j, q has a 1 in column j‑1, and p has a 1
    in column j.  The DP starts with all patterns allowed for the
    first row, and for each additional row the current vector of
    counts is multiplied by the adjacency matrix of allowed
    transitions.  The total number of required arrays for a given n
    is the sum over the last row vector; a(n) is half that number.
    The routine keeps a static cache of previously computed values
    so that calls in arbitrary order are inexpensive.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Test whether transition from pattern p to q is allowed.            */
static bool transition_allowed(uint8_t p, uint8_t q)
{
    for (int j = 1; j < 8; ++j) {
        if ((q >> j) & 1U) {                         /* current cell is 1   */
            if (((q >> (j - 1)) & 1U) &&              /* left neighbour is 1*/
                ((p >> j) & 1U))                     /* above neighbour is 1*/
                return false;                        /* forbidden pattern   */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Grow the cache of computed a(n) values.                            */
static void ensure_cache(idx_t need, seq_t **cache_pp, size_t *cap_pp)
{
    size_t need_cap = (size_t)need + 1;   /* use 1‑based indexing       */
    if (need_cap <= *cap_pp)
        return;
    size_t new_cap = (*cap_pp) ? (*cap_pp) * 2U : 2U;
    while (new_cap < need_cap)
        new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(*cache_pp, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                 /* out of memory – terminate   */
    /* zero the new part of the cache */
    for (size_t i = *cap_pp; i < new_cap; ++i)
        tmp[i] = 0;
    *cache_pp = tmp;
    *cap_pp = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence routine.  (External linkage, as required.)           */
seq_t a(idx_t n)
{
    /* static state                                                 */
    static bool initialized = false;
    static size_t cached_n = 0;           /* largest n already cached       */
    static seq_t *cache = NULL;           /* cache[1..] (1‑based)           */
    static size_t cache_cap = 0;

    static uint16_t *adj_buf = NULL;      /* flat list of destination pattern indices */
    static size_t *adj_start = NULL;      /* start index per source pattern      */
    static size_t total_edges = 0;

    static seq_t dp[2][256];
    static int cur = 0;                    /* index of current row array         */

    /* ------------------------------------------------------------------ */
    /*  initialise adjacency list and cache, once                             */
    if (!initialized) {
        /* Build adjacency list: count edges first */
        size_t edges = 0;
        for (int p = 0; p < 256; ++p)
            for (int q = 0; q < 256; ++q)
                if (transition_allowed((uint8_t)p, (uint8_t)q))
                    ++edges;

        adj_buf = (uint16_t *)malloc(edges * sizeof(uint16_t));
        adj_start = (size_t *)malloc(257 * sizeof(size_t));
        total_edges = edges;

        size_t offset = 0;
        for (int p = 0; p < 256; ++p) {
            adj_start[p] = offset;
            for (int q = 0; q < 256; ++q)
                if (transition_allowed((uint8_t)p, (uint8_t)q))
                    adj_buf[offset++] = (uint16_t)q;
        }
        adj_start[256] = offset;          /* sentinel */

        /* Initialise cache for n = 1 */
        cache_cap = 2;                    /* will hold indices 0 and 1           */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));

        /* DP for first row: all patterns admissible */
        for (int i = 0; i < 256; ++i)
            dp[0][i] = 1;
        seq_t total = 0;
        for (int i = 0; i < 256; ++i)
            total += dp[0][i];
        cache[1] = total / (seq_t)2;     /* a(1) = 128 */
        cached_n = 1;
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /*  Return cached value if already available                            */
    if (n <= cached_n)
        return cache[n];

    /* ------------------------------------------------------------------ */
    /*  Extend DP to requested n (any order of calls)                         */
    seq_t *cur_dp = dp[cur];
    seq_t *next_dp = dp[1 - cur];

    for (idx_t r = cached_n + 1; r <= n; ++r) {
        memset(next_dp, 0, 256 * sizeof(seq_t));

        for (int p = 0; p < 256; ++p) {
            seq_t val = cur_dp[p];
            if (!val)
                continue;
            size_t start = adj_start[p];
            size_t end   = adj_start[p + 1];
            for (size_t k = start; k < end; ++k)
                next_dp[adj_buf[k]] += val;
        }

        seq_t total = 0;
        for (int i = 0; i < 256; ++i)
            total += next_dp[i];

        ensure_cache(r, &cache, &cache_cap);
        cache[r] = total / (seq_t)2;

        /* swap rows for next iteration */
        seq_t *tmp = cur_dp;
        cur_dp = next_dp;
        next_dp = tmp;
        cur = 1 - cur;
        cached_n = r;
    }

    return cache[n];
}
