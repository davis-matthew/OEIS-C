/*
 * A192421  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - a(n-4); initial terms:
 *            a(0)=2, a(1)=0, a(2)=3, a(3)=1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;

seq_t a(idx_t n)
{
    /* Make sure the cache is at least big enough for the four base terms */
    if (cache_capacity < 4) {
        idx_t init_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = init_cap;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)1;
    }

    /* Return a value that has already been computed */
    if (n < cache_capacity) {
        return cache[n];
    }

    /* Grow the cache to include the needs of the requested index */
    idx_t old_cap = cache_capacity;
    idx_t new_cap = n + 1;                      /* index n needs storage for n+1 elements */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_capacity = new_cap;

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        cache[i] =
            cache[i-1] +
            (seq_t)3 * cache[i-2] -
            cache[i-3] -
            cache[i-4];
    }
    return cache[n];
}
