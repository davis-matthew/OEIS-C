/*
 * A213490  Number of (w,x,y) with all terms in {0,...,n} and the numbers
 *          w,x,y,|w-x|,|x-y| distinct.
 * https://oeis.org/A213490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10); a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=12, a(6)=38, a(7)=92,
 *            a(8)=160, a(9)=286
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* exit, EXIT_FAILURE */
#include <stddef.h>   /* size_t */

#include "../utils/seq_alloc.h"     /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t*, size_t, size_t) */

static seq_t *cache = NULL;        /* cache of computed values           */
static size_t cache_cap = 0;       /* allocated capacity of cache         */
static idx_t   max_computed = 0;   /* highest index that has been filled  */

static const seq_t init_vals[10] = {
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL,
    12ULL, 38ULL, 92ULL, 160ULL, 286ULL
};

/* Ensure that the cache can hold indices up to n and that it is initialized
   (the first 10 entries contain the known initial values). */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;                     /* indices 0 … n inclusive */
    if ((size_t)needed <= cache_cap)
        return;                                 /* already big enough */

    seq_t *new_cache;
    if (cache == NULL) {                       /* first allocation */
        new_cache = seq_alloc(needed);          /* may return NULL on error */
        if (!new_cache)
            exit(EXIT_FAILURE);
        /* copy the known initial values */
        for (idx_t i = 0; i < 10 && i < needed; ++i)
            new_cache[i] = init_vals[i];
        /* zero any newly added entries */
        for (idx_t i = 10; i < needed; ++i)
            new_cache[i] = 0;
        cache      = new_cache;
        cache_cap  = needed;
        max_computed = (needed > 9) ? 9 : (idx_t)(needed - 1);
    } else {                                   /* enlarge existing cache */
        new_cache = seq_realloc(cache, cache_cap, needed);
        if (!new_cache)
            exit(EXIT_FAILURE);
        /* initialize only newly added entries to 0 */
        for (idx_t i = cache_cap; i < needed; ++i)
            new_cache[i] = 0;
        cache      = new_cache;
        cache_cap  = needed;
    }
}

/* Public entry point for the OEIS sequence a(n). */
seq_t a(idx_t n)
{
    /* For the first 10 indices the values are known explicitly. */
    if (n < 10)
        return init_vals[n];

    /* Make sure our cache can hold up to index n. */
    ensure_cache(n);

    /* If the requested element has already been computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute all missing values up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 2]
                  - (seq_t)2 * cache[i - 5]
                  + cache[i - 8]
                  + cache[i - 9]
                  - cache[i - 10];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
