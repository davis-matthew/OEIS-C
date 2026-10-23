/*
 * A168993  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(22,n)} 26*a(n-k) + (n <= 22 ? 2 : 0) + (n
 *            == 23 ? 1 - 351*a(0) : 0) - (n >= 24 ? 351*a(n-23) : 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence begins with a(0)=1. */

#include <stddef.h>

/* Helpers provided by the utils library. */
#include "../utils/alloc_seq.h"   /* alloc_seq() */
#include "../utils/free_seq.h"    /* free_seq() */

static seq_t *cache      = NULL;    /* cache of computed values */
static size_t cache_cap  = 0;       /* current allocated capacity (elements) */
static idx_t   last_computed = 0;   /* index of last computed element */

/* Ensure that the cache can hold values up to index n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        /* Copy existing values. */
        for (idx_t i = 0; i < (idx_t)cache_cap; ++i)
            new_cache[i] = cache[i];
        if (cache != NULL)
            free_seq(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    /* Initialise the cache if this is the first call. */
    if (cache == NULL) {
        cache = alloc_seq(1);
        cache[0] = (seq_t)1;
        cache_cap = 1;
        last_computed = 0;
    }

    /* Grow the cache if necessary. */
    ensure_capacity(n);

    /* Compute missing terms up to n. */
    if (last_computed < n) {
        idx_t start = last_computed + 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 0) {
                /* a(0) is already set. */
                continue;
            }
            seq_t sum = 0;
            idx_t maxk = (i < 22 ? i : 22);
            for (idx_t k = 1; k <= maxk; ++k)
                sum += cache[i - k];

            seq_t val;
            if (i <= 22) {
                val = sum * (seq_t)26 + (seq_t)2;
            } else if (i == 23) {
                val = sum * (seq_t)26 + (seq_t)1
                      - (seq_t)351 * cache[0];
            } else {
                val = sum * (seq_t)26
                      - (seq_t)351 * cache[i - 23];
            }
            cache[i] = val;
        }
        last_computed = n;
    }
    return cache[n];
}
