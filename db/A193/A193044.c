/*
 * A193044  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) with
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=5, a(4)=13, a(5)=28
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;   /* allocated size of cache */
static size_t cache_last   = 0;     /* highest index that has been computed */

/* ensure cache is large enough to hold element n */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_capacity >= target) return;          /* already big enough */

    size_t old_cap = cache_capacity;
    size_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap < target) new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif
    if (!new_cache) return;                     /* allocation failed – abort silently */

    /* copy old contents */
    for (size_t i = 0; i < old_cap; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_capacity = new_cap;

    /* initialise base values if cache was empty before */
    if (old_cap == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)28;
        cache_last = 5;
    }
}

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n <= 5) {
        switch ((size_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)2;
            case 3: return (seq_t)5;
            case 4: return (seq_t)13;
            case 5: return (seq_t)28;
        }
    }

    /* ensure cache can store up to n */
    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_last + 1); (size_t)i <= n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t v5 = cache[i-5];
        cache[i] = 4*v1 - 5*v2 + v3 + 2*v4 - v5;
    }
    cache_last = (size_t)n;

    return cache[n];
}
