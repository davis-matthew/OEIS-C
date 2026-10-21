/*
 * A088991  Derangement numbers d(n,4) where d(n,k) = k(n-1)(d(n-1,k) +
 *          d(n-2,k)), with d(0,k) = 1 and d(1,k) = 0.
 * https://oeis.org/A088991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1)*(a(n-1)+a(n-2)), a(0)=1, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for derangement numbers d(n,4) */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max = 0;   /* highest index that has been computed */

/* Ensure the cache can hold up to n and is initialized. */
static void ensure_cache_cap(size_t n) {
    if (cache == NULL) {
        /* initial capacity at least 2 to hold a(0) and a(1) */
        cache_cap = 2;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return;            /* allocation failure – caller will get 0 */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)0;           /* a(1) */
        cache_max = 1;
    }
    while (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* allocation failure – caller will get 0 */
        /* zero any new elements */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
    /* compute any missing values up to n */
    for (size_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t mult = (seq_t)4 * (seq_t)(i - 1);
        cache[i] = sum * mult;
    }
    cache_max = n;
}

seq_t a(idx_t n) {
    /* guard against very large n that exceed SIZE_MAX, though unlikely */
    if (n > (idx_t)(SIZE_MAX - 1)) return (seq_t)0;
    ensure_cache_cap((size_t)n);
    return cache[(size_t)n];
}
