/*
 * A226988  Number of n X 2 0..4 arrays of sums of 2 X 2 subblocks of some
 *          (n+1) X 3 binary array with rows and columns of the latter in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A226988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>12 Initial values (n = 1 …
 *            12): 10, 42, 120, 313, 729, 1556, 3099, 5818, 10384, 17744,
 *            29198, 46489
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 76, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* Static table of the first 12 terms (index 0 unused). */
static const seq_t init_vals[13] = {
    0ULL,   /* unused */
    10ULL,  /* a(1) */
    42ULL,  /* a(2) */
    120ULL, /* a(3) */
    313ULL, /* a(4) */
    729ULL, /* a(5) */
    1556ULL,/* a(6) */
    3099ULL,/* a(7) */
    5818ULL,/* a(8) */
    10384ULL,/* a(9) */
    17744ULL,/* a(10) */
    29198ULL,/* a(11) */
    46489ULL /* a(12) */
};

/* Cached sequence values; 1‑based index, cache[0] unused. */
static seq_t *cache = NULL;
/* Size of the allocated cache array (number of usable indices). */
static idx_t cache_size = 0;
/* Highest n for which a(n) has been computed and stored in cache. */
static idx_t computed_max = 0;

/* Ensure that the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_size)
        return;

    size_t new_len = (size_t)(need + 1);        /* +1 for unused 0 position */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();

    /* Initialize newly allocated slots to 0. */
    for (idx_t i = cache_size + 1; i <= need; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_size = need;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n == 0; return 0 safely */
        return 0;

    /* Direct return for the first 12 known terms. */
    if (n <= 12)
        return init_vals[n];

    /* Make sure storage exists up to n. */
    ensure_cache_size(n);

    /* Seed the cache with the initial 12 values if not already done. */
    if (computed_max < 12) {
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init_vals[i];
        computed_max = 12;
    }

    /* If we already have the value, return it. */
    if (n <= computed_max)
        return cache[n];

    /* Compute terms from the first missing index up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 8ULL * cache[i-1];
        pos   += 56ULL * cache[i-3];
        pos   += 56ULL * cache[i-5];
        pos   += 8ULL  * cache[i-7];

        seq_t neg = 28ULL * cache[i-2];
        neg   += 70ULL * cache[i-4];
        neg   += 28ULL * cache[i-6];
        neg   += 1ULL  * cache[i-8];

        cache[i] = pos - neg;
    }

    computed_max = n;
    return cache[n];
}
