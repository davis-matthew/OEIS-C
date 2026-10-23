/*
 * A020988  a(n) = (2/3)*(4^n-1).
 * https://oeis.org/A020988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A020988: a(0) = 0, a(n) = 4*a(n-1) + 2
 * (also a(n) = (2/3)*(4^n-1))
 */
#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
# include <new>
#endif

/* helpers to allocate/free a seq_t array that work for both C and C++ */

static seq_t *alloc_seq_array(idx_t len) {
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)malloc(len * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* memoized implementation */
seq_t a(idx_t n)
{
    /* static workspace */
    static seq_t *cache = NULL;      /* array of a(0..cache_cap-1) */
    static idx_t cache_cap = 0;      /* capacity of cache array         */
    static idx_t computed_max = 0;   /* highest index that has been computed */

    /* ensure cache exists */
    if (cache == NULL) {
        cache_cap = (n > 0) ? n + 1 : 1;
        cache = alloc_seq_array(cache_cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure -> zero */
        cache[0] = (seq_t)0;                /* a(0) */
        computed_max = 0;
    }
    /* grow cache if needed */
    else if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1; /* grow exponentially */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (new_cache == NULL) return (seq_t)0;
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)4 + (seq_t)2;
    }
    computed_max = n;

    return cache[n];
}
