/*
 * A018054  Expansion of 1/((1-3*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A018054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=21, a(2)=310; for n>2, a(n) = 21*a(n-1) -
 *            131*a(n-2) + 231*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values.  The cache grows on demand so that any call
 * a(n) returns the correct term regardless of call order. */
static seq_t *cache = NULL;   /* dynamically allocated sequence */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* first unfilled index (cache_len == number of stored terms) */

seq_t a(idx_t n)
{
    /* handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)310;

    /* initialise cache if first time */
    if (cache == NULL) {
        cache_cap = 3;
        cache_len = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* abort if allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)310;
    }

    /* grow cache to hold index n if necessary */
    size_t target = (size_t)n;
    if (cache_len <= target) {
        /* grow capacity (double or up to target+1) */
        if (target + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < target + 1) new_cap = target + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= target; ++i) {
            seq_t t1 = 21 * cache[i - 1];
            seq_t t2 = 131 * cache[i - 2];
            seq_t t3 = 231 * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_len = target + 1;
    }

    return cache[n];
}
