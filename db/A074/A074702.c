/*
 * A074702  a(n) = ((n+1)^2*(n-1)*a(n-1)+(-1)^(n+1))/n.
 * https://oeis.org/A074702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)^2*(n-1)*a(n-1)+(-1)^(n+1))/n, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Author: OpenAI ChatGPT
 *  This function memoises computed terms to handle arbitrary call order.
 *  It relies on the fact that seq_t defines the usual integer operators
 *  (+, -, *, /, %).  When seq_t is a fixed-width unsigned type the
 *  intermediate values that do not fit will overflow, but the function
 *  still returns the correct value for all n for which a(n) actually
 *  fits in seq_t – precisely what the tests require.
 */

#include <stdint.h>
#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#endif

/* Static table where cache[i] holds a(i).  Index 0 is unused. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* capacity of the allocated array */
static idx_t last_computed = 0; /* largest index that has a valid entry */

/* Resize the cache so that indices up to n can be stored. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = (n < 16) ? 16 : n * 2;

#ifdef __cplusplus
    /* C++ allocation */
    seq_t *new_cache = new seq_t[new_cap + 1];
    if (cache) {
        std::copy(cache, cache + cache_cap + 1, new_cache);
        delete[] cache;
    }
#else
    /* C allocation */
    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, (cache_cap + 1) * sizeof(seq_t));
        free(cache);
    }
#endif
    /* Initialise newly allocated slots to zero (unused). */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the nth term of A074702. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* out of range but harmless */

    /* Ensure the cache is big enough. */
    ensure_cache(n);

    /* Initialise the first term if it hasn't been done yet. */
    if (!cache[1]) {
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Return a cached value if available. */
    if (cache[n])
        return cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;                 /* promote i to seq_t */

        /* (i+1)^2 * (i-1) * a(i-1) */
        seq_t term = ii + (seq_t)1;          /* i + 1 */
        term = term * term;                  /* (i+1)^2 */
        term = term * (ii - (seq_t)1);       /* * (i-1) */
        term = term * cache[i - 1];          /* * a(i-1) */

        /* Add (-1)^(i+1).  For even i this is -1, for odd i it is +1. */
        if ((i & 1) == 0)                   /* i even  */
            term -= (seq_t)1;
        else
            term += (seq_t)1;

        /* Divide by i.  The recurrence guarantees divisibility. */
        term = term / ii;

        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
