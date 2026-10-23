/*
 * A015576  Expansion of x/(1 - 8*x - 7*x^2).
 * https://oeis.org/A015576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 7*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

/* Ensure the cache has room for at least new_cap terms. */
static void ensure_cache_cap(size_t new_cap) {
    if (cache_cap >= new_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a production environment you would handle the error; here we
           simply abort to keep the function correct for the tests. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A015576. */
seq_t a(idx_t n) {
    /* Handle the first two terms quickly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Make sure the cache has space for at least n+1 elements. */
    ensure_cache_cap((size_t)n + 1);

    /* Initialise the cache on the first real call that needs it. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    } else if (cache_len == 1) {
        /* This can only happen if a(0) was the first call; still initialise. */
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute missing terms up to n. */
    if (cache_len <= (size_t)n) {
        size_t i;
        for (i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] + (seq_t)7 * cache[i - 2];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
