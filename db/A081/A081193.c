/*
 * A081193  a(n) = 6*a(n-1)-8*a(n-2) for n>1, a(0)=1, a(1)=9.
 * https://oeis.org/A081193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A081193
   Formula: a(n) = 6*a(n-1) - 8*a(n-2)   (n > 1)
   Initial values: a(0) = 1, a(1) = 9
   This implementation memoises terms in a static array.
   It works for any seq_t and idx_t that satisfy the assumed
   integer arithmetic and size_t compatible types. */

#include <stdlib.h>    /* realloc, NULL */

static seq_t *cache = NULL;            /* dynamic array of computed terms */
static idx_t  cache_size  = 0;         /* number of terms stored          */
static idx_t  cache_capacity = 0;      /* allocated capacity of cache     */

/* initialise the cache with the first two terms */
static void init_cache(void)
{
    if (cache_capacity < 2) {
        idx_t new_cap = 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }
    cache[0] = 1;
    cache[1] = 9;
    cache_size = 2;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_size == 0) {
        init_cache();
    }

    /* grow cache if required */
    if (n >= cache_size) {
        idx_t new_cap = cache_capacity;
        /* double until it accommodates n+1 */
        while (new_cap <= n) new_cap <<= 1;
        if (new_cap == 0) new_cap = 1;          /* safety for n=0 */

        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            t1 = t1 * 6;          /* 6 * a(i-1) */
            seq_t t2 = cache[i - 2];
            t2 = t2 * 8;          /* 8 * a(i-2) */
            cache[i] = t1 - t2;   /* a(i) = 6*a(i-1) - 8*a(i-2) */
        }
        cache_size = n + 1;
    }

    return cache[n];
}
