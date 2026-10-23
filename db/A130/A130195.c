/*
 * A130195  Row sums of triangle A130194.
 * https://oeis.org/A130195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)-2*a(n-2)+2*a(n-3)-a(n-4)+a(n-5); Initial values:
 *            a(1)=1, a(2)=3, a(3)=2, a(4)=-2, a(5)=-1 Offset: 1
 * Range:     i8 n <= 62, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for computed terms: cache[i] holds a(i+1). */
static seq_t *cache = NULL;      /* dynamic array */
static size_t cache_cap   = 0;   /* allocated capacity (number of elements) */
static size_t cache_filled = 0;  /* number of terms already computed */

/* Ensure the cache can hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : needed;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute terms up to n (1‑based).  cache_filled is the count of terms stored. */
static void ensure_terms(idx_t n)
{
    if (n < 1) return;                     /* nothing to do for n <= 0 */
    size_t needed = (size_t)n;
    if (cache_filled >= needed) return;    /* already computed */

    grow_cache(needed);

    for (size_t i = cache_filled; i < needed; ++i)
    {
        seq_t val;
        /* Base cases: a(1) … a(5) */
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)3;
        else if (i == 2)     val = (seq_t)2;
        else if (i == 3)     val = (seq_t)-2;
        else if (i == 4)     val = (seq_t)-1;
        else
        {
            /* a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) */
            val =    cache[i-1]
                  - (seq_t)2 * cache[i-2]
                  + (seq_t)2 * cache[i-3]
                  - cache[i-4]
                  + cache[i-5];
        }
        cache[i] = val;
    }
    cache_filled = needed;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety. */
    if (n < 1) return (seq_t)0;
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
