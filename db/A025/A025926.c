/*
 * A025926  Expansion of 1/((1-x^10)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025926 (Partitions into 10, 11 and 12)
   Formula: a(n)=a(n-10)+a(n-11)+a(n-12)-a(n-21)-a(n-22)-a(n-23)+a(n-33),
   with a(0)=1 and a(n)=0 for 1≤n≤9. */

#include <stdlib.h>
#include <stddef.h>

/* Ensure that the cache contains at least (n+1) elements.
   The cache and its capacity are updated as needed. */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0 ? 1 : *cap);
    while (new_cap < needed)
        new_cap <<= 1;                  /* exponential growth */
    /* Cast from void* to seq_t* so the code compiles under C++ too */
    *cache = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* computed values */
    static size_t cache_cap  = 0;      /* allocated size */
    static idx_t  max_computed = 0;    /* largest n computed */

    /* First call: initialise the cache with a(0)=1 */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = 1;
        max_computed = 0;
    }

    /* Make sure the cache is large enough for index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing values up to n (in any call order) */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 10) val += cache[i - 10];
            if (i >= 11) val += cache[i - 11];
            if (i >= 12) val += cache[i - 12];
            if (i >= 21) val -= cache[i - 21];
            if (i >= 22) val -= cache[i - 22];
            if (i >= 23) val -= cache[i - 23];
            if (i >= 33) val += cache[i - 33];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
