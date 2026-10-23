/*
 * A037644  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1.
 * https://oeis.org/A037644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n) = 9*a(n-1) + pattern[(n-1)%3],
   where pattern = {2, 3, 1}.  Thus a(1)=2, a(2)=21, a(3)=190, … */
#include <stddef.h>
#include <stdlib.h>

static const int pattern[3] = {2, 3, 1};

seq_t a(idx_t n)
{
    /* a(0) is defined as 0; for safety return 0 if n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed values, cache[0] = 0 */
    static size_t cache_len = 0;  /* number of elements currently stored (>=1) */
    static size_t cache_cap = 0;  /* current allocated capacity */

    /* Initialise cache if this is the first call */
    if (cache_len == 0) {
        cache_cap = 16;  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;      /* allocation fail – fall back to 0 */
        cache[0] = (seq_t)0;      /* a(0) = 0 */
        cache_len = 1;           /* we have 1 element stored */
    }

    /* Ensure capacity for all indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation fail – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t value = prev * (seq_t)9 + (seq_t)pattern[(size_t)(i - 1) % 3];
        cache[(size_t)i] = value;
    }
    cache_len = (size_t)n + 1;  /* update the stored length */

    return cache[(size_t)n];
}
