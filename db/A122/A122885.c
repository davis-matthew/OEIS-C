/*
 * A122885  a(n+1) = 4*a(n) + 11*a(n-1) - 2*a(n-2).
 * https://oeis.org/A122885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 4*a(n) + 11*a(n-1) - 2*a(n-2) Initial values: a(1)=1,
 *            a(2)=13, a(3)=61
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

/* Allocate a new array of seq_t with the given capacity. */
static seq_t *alloc_cache(size_t cap) {
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* Resize the cache to new_cap, copying the existing elements. */
static seq_t *realloc_cache(seq_t *old, size_t old_size, size_t new_cap) {
#if defined(__cplusplus)
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < old_size; ++i) new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    (void)old_size; /* unused in C */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return the n-th term of the sequence, with a(1) being the first term. */
seq_t a(idx_t n) {
    if (n < 1U) return (seq_t)0;        /* undefined for n < 1 */

    /* On first call initialise the cache with the first three terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;        /* number of terms currently stored */

    if (cache_len == 0) {
        cache_cap = 8;                  /* starting capacity */
        cache     = alloc_cache(cache_cap);

        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)13;           /* a(2) */
        cache[2] = (seq_t)61;           /* a(3) */
        cache_len = 3;
    }

    /* Grow the array if we need more terms. */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n) new_cap <<= 1;
            cache = realloc_cache(cache, cache_len, new_cap);
            cache_cap = new_cap;
        }

        /* Compute the missing terms using the recurrence. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* i corresponds to index i+1 */
            seq_t val = (seq_t)4 * cache[i - 1] + (seq_t)11 * cache[i - 2]
                        - (seq_t)2 * cache[i - 3];
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
