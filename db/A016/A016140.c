/*
 * A016140  Expansion of 1/((1-3*x)*(1-8*x)).
 * https://oeis.org/A016140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-2), a(0) = 1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc */

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t  cache_len = 0;  /* number of values actually stored (<= cache_cap) */

/* Compute a(n) for the expansion of 1/((1-3*x)*(1-8*x)) */
seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;                    /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* malloc failure – return 0 (unlikely) */
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = (seq_t)11;             /* a(1) */
        cache_len = 2;
    }

    /* Return early for the two known values */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Ensure capacity for up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* realloc failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 11 * cache[i-1] - 24 * cache[i-2];
    }
    cache_len = n + 1;   /* now we have values up to index n */

    return cache[n];
}
