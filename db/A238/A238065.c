/*
 * A238065  Number of (n+1)X(1+1) 0..3 arrays with the maximum minus the
 *          minimum of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A238065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-2) + 689*a(n-4) + 1889*a(n-6) + 1784*a(n-8) +
 *            512*a(n-10) for n > 13; initial values a(1) .. a(13) are the
 *            first thirteen terms of the sequence. The function computes a(n)
 *            for any n ≥ 1, memoising results so that repeated calls are fast
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1 (offset 1). */
    if (n < 1) {
        /* Undefined below the offset; return 0 as safe default. */
        return (seq_t)0;
    }

    /* Known first 13 terms; they are hard‑coded constants because the
       recurrence requires them as initial conditions.  They are not part of
       a lookup table for further terms and are used only to start the
       recurrence. */
    static const seq_t init[13] = {
        /* a(1) .. a(13) */
        256ULL,
        1644ULL,
        10700ULL,
        67056ULL,
        438768ULL,
        2752364ULL,
        17963540ULL,
        112686608ULL,
        736140896ULL,
        4617797164ULL,
        30156164612ULL,
        189170092912ULL,
        1235514228944ULL
    };

    /* Static cache for memoisation.  cache[i] holds a(i+1).  cache_len is
       the number of stored elements. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache with the first 13 known values on first use. */
    if (cache == NULL) {
        cache_len = 13;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        memcpy(cache, init, cache_len * sizeof(seq_t));
    }

    /* If the requested n is already in the cache, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand the cache to accommodate up to n.  The growth is done in a
       single reallocation; the new slots are filled using the recurrence. */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* allocation failure */
    cache = tmp;

    /* Compute new entries using the empirical linear recurrence.  We use
       64‑bit unsigned arithmetic if seq_t is 64 bits; for larger seq_t
       (uint128 or big integer) the same expression is valid. */
    size_t i;
    for (i = cache_len; i < new_len; ++i) {
        /* i is zero‑based index corresponding to a(i+1).  The recurrence
           accesses terms a(n-2), a(n-4), ... where n = i+1. */
        seq_t t1 = cache[i - 2];   /* a(n-2) */
        seq_t t2 = cache[i - 4];   /* a(n-4) */
        seq_t t3 = cache[i - 6];   /* a(n-6) */
        seq_t t4 = cache[i - 8];   /* a(n-8) */
        seq_t t5 = cache[i -10];   /* a(n-10) */
        cache[i] = (seq_t)23 * t1
                 + (seq_t)689 * t2
                 + (seq_t)1889 * t3
                 + (seq_t)1784 * t4
                 + (seq_t)512 * t5;
    }

    cache_len = new_len;
    return cache[n - 1];
}
