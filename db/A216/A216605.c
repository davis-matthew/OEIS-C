/*
 * A216605  Expansion of g.f. (6 + 5*x - 20*x^2 - 12*x^3 + 12*x^4 + 3*x^5)/(1
 *          + x - 5*x^2 - 4*x^3 + 6*x^4 + 3*x^5 - x^6).
 * https://oeis.org/A216605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 5*a(n-2) + 4*a(n-3) - 6*a(n-4) - 3*a(n-5) +
 *            a(n-6) Initial values: a(0)=6, a(1)=-1, a(2)=11, a(3)=-4,
 *            a(4)=31, a(5)=-16
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 65, i128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cache of computed values */
static idx_t  cache_size = 0;      /* how many values are valid in cache */
static size_t cache_cap  = 0;      /* allocated size of cache */

/* Expand the cache if necessary so that it can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                 /* already have this value */

    /* Allocate or grow the cache */
    if (cache == NULL) {
        /* Initial allocation: enough for n+1 elements */
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        /* Grow, at least double the capacity or sufficiently large */
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)      cache[0] = (seq_t)  6;
        else if (i == 1) cache[1] = (seq_t) -1;
        else if (i == 2) cache[2] = (seq_t) 11;
        else if (i == 3) cache[3] = (seq_t) -4;
        else if (i == 4) cache[4] = (seq_t) 31;
        else if (i == 5) cache[5] = (seq_t) -16;
        else {
            /* recurrence: a(n) = -a(n-1) + 5*a(n-2) + 4*a(n-3) - 6*a(n-4) - 3*a(n-5) + a(n-6) */
            cache[i] =
                -cache[i-1]
                + (seq_t)5 * cache[i-2]
                + (seq_t)4 * cache[i-3]
                - (seq_t)6 * cache[i-4]
                - (seq_t)3 * cache[i-5]
                +        cache[i-6];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)(-1)) return (seq_t)0;   /* safety for underflow when n-1 used */
    ensure_cache(n);
    return cache[n];
}
