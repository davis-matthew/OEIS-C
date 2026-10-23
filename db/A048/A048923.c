/*
 * A048923  Indices of octagonal numbers which are also 9-gonal.
 * https://oeis.org/A048923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 459 a(n) = 674*a(n-1) - a(n-2) - 224 (for n >=
 *            3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of terms, zero‑based (cache[0] = a(1)) */
static idx_t cache_count = 0;   /* number of computed terms (max index cached) */
static idx_t cache_capacity = 0;/* allocated array length */

/* Return the nth term of A048923 (1‑based n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* n must be >= 1 in normal use */

    /* Allocate / enlarge the cache if necessary */
    if (n > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 32;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_count + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if (i == 2) {
            val = 459;
        } else {
            seq_t t = (seq_t)674 * cache[i - 2]; /* 674*a(i-1) */
            t -= cache[i - 3];                   /* -a(i-2)   */
            t -= 224;                            /* -224       */
            val = t;
        }
        cache[i - 1] = val;  /* store a(i) at index i-1 */
    }
    cache_count = n;          /* update count of computed terms */
    return cache[n - 1];      /* return a(n) */
}
