/*
 * A234684  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,…,6} in which * for
 *            every 2×2 subblock the difference between its diagonal sum and *
 *            antidiagonal sum equals ±5 (absolute value 5). The sequence is *
 *            indexed from 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   pair_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pair_index.h"

#define DIFF_MIN   (-6)
#define DIFF_MAX   (6)
#define DIFF_RANGE (DIFF_MAX - DIFF_MIN + 1)   /* 13 */
#define STATE_COUNT (DIFF_RANGE * DIFF_RANGE)   /* 169 */

/* Transition data */
static unsigned char N[STATE_COUNT];                      /* number of rows with given differences */
static unsigned char out_count[STATE_COUNT];              /* number of reachable next states (≤4) */
static int out_index[STATE_COUNT][4];                    /* indices of reachable next states */

/* Cached sequence values: cache[n] holds a(n).  We only allocate when needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;         /* allocated size of cache array */

/* Highest n that has been computed and stored in cache. */
static idx_t cached_max = 0;

/* Current number of processed transitions (equals the highest n computed). */
static idx_t current_level = 0;

/* Cached transition matrix is computed once. */
static bool tables_initialized = false;

/* ------------------------------------------------------------------ */
/*  Build the transition tables.                                      */
static void init_tables(void)
{
    if (tables_initialized)
        return;

    /* Precompute N[d1,d2] */
    for (int d1 = DIFF_MIN; d1 <= DIFF_MAX; ++d1)
    {
        for (int d2 = DIFF_MIN; d2 <= DIFF_MAX; ++d2)
        {
            int L = 0;
            if (-d1 > L) L = -d1;          /* ensure s1 ≥ -d1 */
            if (d2 > L) L = d2;            /* ensure s1 ≥ d2 */
            int U = 6;
            if (6 - d1 < U) U = 6 - d1;    /* ensure s1 ≤ 6 - d1 */
            if (6 + d2 < U) U = 6 + d2;    /* ensure s1 ≤ 6 + d2 */
            unsigned char cnt = 0;
            if (U >= L) cnt = (unsigned char)(U - L + 1);
            N[pair_index(d1, d2)] = cnt;
        }
    }

    /* Precompute transitions for each state */
    for (int d1 = DIFF_MIN; d1 <= DIFF_MAX; ++d1)
    {
        for (int d2 = DIFF_MIN; d2 <= DIFF_MAX; ++d2)
        {
            int src = pair_index(d1, d2);
            unsigned char cnt = 0;
            for (int s1 = -5; s1 <= 5; s1 += 10)          /* -5 or +5 */
            {
                for (int s2 = -5; s2 <= 5; s2 += 10)      /* -5 or +5 */
                {
                    int d1t = d1 + s1;
                    int d2t = d2 + s2;
                    if (d1t < DIFF_MIN || d1t > DIFF_MAX ||
                        d2t < DIFF_MIN || d2t > DIFF_MAX)
                        continue;
                    out_index[src][cnt++] = pair_index(d1t, d2t);
                }
            }
            out_count[src] = cnt;
        }
    }

    tables_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the cache is filled up to index n (inclusive).        */
static void ensure_cache_upto(idx_t n)
{
    init_tables();

    if (n <= cached_max)
        return;

    /* Allocate or grow the cache array */
    if ((size_t)(n + 1) > cache_len)
    {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* allocation failed; abort */
            exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    /* DP arrays: f_prev holds counts after current_level transitions,
       f_cur holds counts for the next level */
    static seq_t f_prev[STATE_COUNT];
    static seq_t f_cur[STATE_COUNT];
    static bool dp_initialized = false;

    if (!dp_initialized)
    {
        /* Initial level (0 transitions): f_prev holds counts of 1-row arrays
           for each state (differences). */
        for (int i = 0; i < STATE_COUNT; ++i)
            f_prev[i] = (seq_t)N[i];
        /* Cache a(0) as 0; not used. */
        cache[0] = (seq_t)0;
        dp_initialized = true;
    }

    /* Process levels from current_level up to n-1. */
    for (idx_t lv = current_level; lv < n; ++lv)
    {
        /* Reset f_cur */
        for (int i = 0; i < STATE_COUNT; ++i)
            f_cur[i] = (seq_t)0;

        /* Transition */
        for (int src = 0; src < STATE_COUNT; ++src)
        {
            seq_t val = f_prev[src];
            if (val == (seq_t)0)
                continue;
            unsigned char cnt = out_count[src];
            for (unsigned char t = 0; t < cnt; ++t)
            {
                int tgt = out_index[src][t];
                f_cur[tgt] += val * (seq_t)N[tgt];
            }
        }

        /* Sum over all states to get a(lv+1) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < STATE_COUNT; ++i)
            total += f_cur[i];
        cache[lv + 1] = total;

        /* Prepare for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i)
            f_prev[i] = f_cur[i];
    }

    current_level = n;
    cached_max = n;
}

/* ------------------------------------------------------------------ */
/*  Public function to compute a(n).                                   */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_cache_upto(n);
    return cache[n];
}
