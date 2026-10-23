/*
 * A024516  Positions of odd numbers in A000404 (sums of 2 nonzero squares).
 * https://oeis.org/A024516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   max_seq_t_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute positions of odd numbers in the sorted list of sums
   of two non‑zero squares (OEIS A024516).  The function generates
   all sums <= a growing bound, sorts and deduplicates them,
   then records the positions (1‑based) of the odd sums.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/max_seq_t_value.h"

/* Comparer for qsort on seq_t values */
static int seq_cmp(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Global cache of positions of odd numbers.
   cache[0] is unused; cache[1] = a(1), cache[2] = a(2), … */
static seq_t *cache = NULL;        /* allocated with cache_len+1 elems */
static size_t cache_len = 0;       /* number of valid terms (a(1)..a(cache_len)) */
static idx_t  current_limit = 0;   /* largest sum value already processed */

/* Compute all odd positions for sums <= limit and rebuild the cache. */
static void recompute_cache(idx_t limit) {
    /* Estimate number of (i,j) pairs: (sqrt(limit)+1)^2/2 + few */
    idx_t sqrt_limit = 0;
    /* simple integer sqrt */
    while ((unsigned long long)(sqrt_limit + 1) * (sqrt_limit + 1) <= (unsigned long long)limit)
        ++sqrt_limit;
    size_t est = (size_t)(sqrt_limit + 1) * (size_t)(sqrt_limit + 1) / 2 + 5;

    seq_t *sums = (seq_t *)malloc(est * sizeof(seq_t));
    if (!sums) {
        /* Allocation failed – terminate */
        abort();
    }
    size_t sz = 0;

    /* Generate all sums i^2 + j^2, 1 <= i <= j, i^2 + j^2 <= limit */
    for (idx_t i = 1; ; ++i) {
        unsigned long long i2 = (unsigned long long)i * i;
        if (i2 > (unsigned long long)limit) break;
        for (idx_t j = i; ; ++j) {
            unsigned long long sum = i2 + (unsigned long long)j * j;
            if (sum > (unsigned long long)limit) break;
            if (sz == est) {
                est <<= 1;
                sums = (seq_t *)realloc(sums, est * sizeof(seq_t));
                if (!sums) abort();
            }
            sums[sz++] = (seq_t)sum;
        }
    }

    /* Sort and deduplicate */
    qsort(sums, sz, sizeof(seq_t), seq_cmp);
    size_t uniq = 0;
    seq_t prev = 0;
    bool first = true;

    /* Count odd sums to know final capacity */
    for (size_t k = 0; k < sz; ++k) {
        seq_t cur = sums[k];
        if (first || cur != prev) {
            first = false;
            prev = cur;
            if ((cur & 1) != 0) {
                ++uniq;
            }
        }
    }

    /* Allocate new cache (+1 for unused 0 index) */
    seq_t *new_cache = (seq_t *)calloc(uniq + 1, sizeof(seq_t));
    if (!new_cache) abort();

    /* Second pass to store positions of odd sums */
    size_t position = 0;   /* 1‑based position in the sorted list */
    first = true;
    prev = 0;
    size_t odd_idx = 0;
    for (size_t k = 0; k < sz; ++k) {
        seq_t cur = sums[k];
        if (first || cur != prev) {
            first = false;
            prev = cur;
            ++position;
            if ((cur & 1) != 0) {
                new_cache[odd_idx + 1] = (seq_t)position;
                ++odd_idx;
            }
        }
    }

    free(sums);
    free(cache);
    cache = new_cache;
    cache_len = uniq;
    current_limit = limit;
}

/* Ensure that at least 'need' odd positions have been computed. */
static void ensure_cache(idx_t need) {
    if (need == 0) return;
    if (cache_len >= (size_t)need) return;

    unsigned long long max_val = max_seq_t_value();
    if (current_limit == 0) current_limit = 1;

    /* Grow the search limit until we have enough odd positions */
    while (cache_len < (size_t)need) {
        /* Double the limit but don't exceed max value of seq_t */
        idx_t next_limit;
        if (current_limit > max_val / 2) {
            next_limit = max_val;
        } else {
            next_limit = current_limit * 2;
        }
        if (next_limit <= current_limit) {
            /* We cannot grow further – break to avoid infinite loop */
            break;
        }
        recompute_cache(next_limit);
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}
