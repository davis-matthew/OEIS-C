/*
 * A157128  Expansion of (1 - x - x^2 + x^3 - x^5) / ((1 + x)^2*(1 - x +
 *          x^2)^2).
 * https://oeis.org/A157128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-1, a(2)=-1, a(3)=-1, a(4)=2, a(5)=1 For n > 5:
 *            a(n) = -2*a(n-3) - a(n-6) (OEIS A157128) Computes directly using
 *            memoized recurrence
 * Range:     i8 n <= 381, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t a_init[6] = {1,-1,-1,-1,2,1};
static seq_t *a_cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* return small fixed values directly */
    if (n < 6u) return a_init[(size_t)n];

    /* first time we need the cache, allocate up to 6 */
    if (!a_cache) {
        cache_size = 6;
        a_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!a_cache) return 0;
        for (size_t i = 0; i < 6; ++i) a_cache[i] = a_init[i];
    }

    size_t idx = (size_t)n;
    if (idx < cache_size) return a_cache[idx];

    /* extend cache to accommodate n */
    size_t new_size = idx + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;             /* out of memory, return 0 as safe fallback */
    a_cache = tmp;
    for (size_t i = cache_size; i <= idx; ++i) {
        /* recurrence: a(i) = -2*a(i-3) - a(i-6) */
        a_cache[i] = (seq_t)-2 * a_cache[i - 3] - a_cache[i - 6];
    }
    cache_size = new_size;
    return a_cache[idx];
}
