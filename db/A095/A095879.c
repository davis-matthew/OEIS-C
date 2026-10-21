/*
 * A095879  Numbers whose lazy Fibonacci representation has an odd number of
 *          summands.
 * https://oeis.org/A095879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A095879: numbers whose lazy Fibonacci representation has an
 * odd number of summands.
 * The function a(idx_t n) returns the n-th positive integer (n ≥ 1)
 * having this property.  It generates values on demand and resizes
 * its internal table geometrically.  The implementation uses an
 * exhaustive DP for sums up to the current limit; only the value
 * type seq_t, as defined by the build, is used. */
#include <stdlib.h>
#include <limits.h>

static seq_t *cache = NULL;    /* terms of the sequence */
static size_t cache_len = 0;   /* number of stored terms */
static idx_t limit = 0;        /* maximum sum that has been processed */

/* --------------------------------------------------------------------
   Recompute all lazy-Fibonacci representations up to new_limit.
   The array 'cache' is replaced by a new array containing exactly
   those numbers s (1 ≤ s ≤ new_limit) whose representation uses an
   odd number of distinct Fibonacci numbers (1,2,3,5,…).  The DP
   keeps, for each sum, the minimal largest Fibonacci index that
   participates and, among those, the maximal number of summands.
   -------------------------------------------------------------------- */
static void recompute(idx_t new_limit)
{
    /* Construct Fibonacci numbers ≤ new_limit (starting 1,2) */
    seq_t *fib = (seq_t *)malloc(sizeof(seq_t) * 32);
    int fib_cnt = 0;
    fib[fib_cnt++] = (seq_t)1;
    if (new_limit >= 2) fib[fib_cnt++] = (seq_t)2;
    while (1) {
        seq_t last = fib[fib_cnt - 1];
        seq_t second = fib[fib_cnt - 2];
        seq_t nxt = last + second;
        if (nxt > new_limit) break;
        fib[fib_cnt++] = nxt;
    }

    /* DP arrays (size new_limit+1) */
    size_t sz = (size_t)new_limit + 1;
    int *best_max = (int *)malloc(sz * sizeof(int));
    int *best_cnt = (int *)malloc(sz * sizeof(int));
    best_max[0] = 0;
    best_cnt[0] = 0;
    for (size_t i = 1; i < sz; ++i) {
        best_max[i] = INT_MAX;
        best_cnt[i] = 0;
    }

    /* Process Fibonacci numbers in increasing order */
    for (int i = 0; i < fib_cnt; ++i) {
        seq_t f = fib[i];
        size_t fsz = (size_t)f;
        /* Descending loop over sums to avoid reuse */
        for (size_t s = (size_t)new_limit; ; --s) {
            if (s < fsz) break;
            if (best_max[s - fsz] != INT_MAX) {
                int cand_max = best_max[s - fsz];
                if (i > cand_max) cand_max = i;
                int cand_cnt = best_cnt[s - fsz] + 1;
                if (cand_max < best_max[s] ||
                    (cand_max == best_max[s] && cand_cnt > best_cnt[s])) {
                    best_max[s] = cand_max;
                    best_cnt[s] = cand_cnt;
                }
            }
            if (s == fsz) break;
        }
    }

    /* Build cache of numbers with odd count of summands */
    seq_t *new_cache = (seq_t *)malloc(sz * sizeof(seq_t));
    size_t new_len = 0;
    for (size_t s = 1; s < sz; ++s) {
        if (best_cnt[s] & 1) {
            new_cache[new_len++] = (seq_t)s;
        }
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_len = new_len;
    limit = new_limit;

    /* Free temporary arrays */
    free(best_max);
    free(best_cnt);
    free(fib);
}

/* --------------------------------------------------------------------
   Public function required by the framework.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms precomputed.  Start with 1024
     * and double afterwards to keep the number of recomputations low. */
    while (cache_len < (size_t)n) {
        idx_t new_limit = (limit == 0) ? (idx_t)1024 : limit * 2;
        recompute(new_limit);
    }
    return cache[(size_t)(n - 1)];
}
