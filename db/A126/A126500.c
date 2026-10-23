/*
 * A126500  Number of base 32 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126500: Number of base‑32 n‑digit numbers with adjacent digits differing by
 * at most three.
 * Recurrence:
 *   dp[1][d] = 1                                 for all 0 ≤ d ≤ 31
 *   dp[k][d] = Σ_{j=max(0,d-3)}^{min(31,d+3)} dp[k-1][j]   for k ≥ 2
 *   a(n)     = Σ_{d=0}^{31} dp[n][d]             for n ≥ 1
 *   a(0)     = 1
 *
 * The implementation memoises computed values so that subsequent calls for any
 * n retrieve the answer in O(1), and incremental computation for larger n
 * takes O(n) time and O(1) extra space.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The integer types are supplied by the build environment. */

/* Forward declaration of the sequence function. */
seq_t a(idx_t n);

/* Memoisation state */
static seq_t   *cache = NULL;      /* array of computed a(n) values   */
static size_t   cap   = 0;         /* capacity of cache               */
static idx_t    max_n = 0;         /* highest n computed so far       */

/* Auxiliary dynamic‑programming storage */
static seq_t dp_prev[32];          /* dp for current length k         */
static seq_t dp_cur [32];          /* dp for length k+1               */
static bool   dp_inited = false;   /* true after dp_prev has been set */

/* Resize the cache to accommodate values up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – the environment will treat this as
             * failure to compute the term. Return zero as a sentinel. */
            return;
        }
        cache = new_cache;
        cap   = new_cap;
    }
}

/* Initialise the DP tables for length 1 (dp_prev). */
static void init_dp_length_one(void)
{
    for (int d = 0; d < 32; ++d) {
        dp_prev[d] = (seq_t)1;
    }
    /* a(1) is the sum of dp_prev. */
    seq_t total = 0;
    for (int d = 0; d < 32; ++d) {
        total += dp_prev[d];
    }
    if (!cache) {
        /* Cache size should already be at least 2 by fixup. */
        ensure_cache_cap(1);
    }
    cache[1] = total;
    max_n = 1;
    dp_inited = true;
}

/* Compute a(n) for any required n, memoising results. */
seq_t a(idx_t n)
{
    /* Handle the base case a(0)=1. */
    if (n == 0) {
        ensure_cache_cap(0);
        if (!cache || cap < 1) return (seq_t)0;  /* safety */
        cache[0] = (seq_t)1;
        max_n = 0;
        return cache[0];
    }

    /* Allocate/cache storage for the requested n. */
    ensure_cache_cap(n);

    /* If the value is already cached, return it. */
    if (n <= max_n) {
        return cache[n];
    }

    /* Initialise DP tables if this is the first non‑zero computation. */
    if (!dp_inited) {
        init_dp_length_one();
    }

    /* Incrementally compute lengths from max_n+1 up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* For k == 1 we already computed dp_prev. */
        if (k > 1) {
            /* Compute dp_cur[d] for all d using dp_prev. */
            for (int d = 0; d < 32; ++d) {
                int low  = d - 3;
                int high = d + 3;
                if (low  < 0)  low  = 0;
                if (high > 31) high = 31;
                seq_t sum = 0;
                for (int j = low; j <= high; ++j) {
                    sum += dp_prev[j];
                }
                dp_cur[d] = sum;
            }
            /* Copy dp_cur into dp_prev for the next iteration. */
            for (int d = 0; d < 32; ++d) {
                dp_prev[d] = dp_cur[d];
            }
        }

        /* Sum dp_prev to obtain a(k). */
        seq_t total = 0;
        for (int d = 0; d < 32; ++d) {
            total += dp_prev[d];
        }
        cache[k] = total;
    }

    max_n = n;
    return cache[n];
}
