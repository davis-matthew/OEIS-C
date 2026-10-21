/*
 * A077912  Expansion of 1/(1+x^2-2*x^3).
 * https://oeis.org/A077912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 64, i64 n <= 126,
 *            i128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 0, a(2) = -1;
 * a(n) = -a(n-2) + 2*a(n-3)  for n >= 3
 */

#include <stdlib.h>

/* Dynamic cache for computed terms */
static seq_t *cache   = NULL;   /* array storing terms */
static idx_t  cache_len = 0;    /* number of terms currently stored */
static idx_t  cache_cap = 0;    /* current capacity of the cache */

/* Grow the cache so that it can hold at least 'needed' elements. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;

    /* Cast the result of realloc to seq_t* for C++ compatibility */
    seq_t *new_ptr = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* allocation failure */

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure base terms are initialized */
    if (cache_len == 0) {
        grow_cache(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-1;
        cache_len = 3;
    }

    /* If the term is already known, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to n and compute missing terms */
    grow_cache(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Using recurrence: a[i] = -a[i-2] + 2*a[i-3] */
        cache[i] = -cache[i-2] + (seq_t)2 * cache[i-3];
    }
    cache_len = n + 1;
    return cache[n];
}
