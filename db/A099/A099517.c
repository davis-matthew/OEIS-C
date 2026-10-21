/*
 * A099517  A transform of (1-x)/(1-2x).
 * https://oeis.org/A099517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099517: a(n) = 2*a(n-1)-2*a(n-3)+2*a(n-4)-a(n-6)
   with a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=10. */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n) {
    /* static cache holding computed values */
    static seq_t *cache     = NULL;  /* array of seq_t */
    static size_t cache_cap = 0;     /* allocated capacity (bytes/elem) */
    static size_t cache_n  = 0;      /* number of entries cached (highest index+1) */

    /* ensure we can index into cache with n */
    if ((size_t)n >= SIZE_MAX) return 0;   /* out of bounds for array index */

    /* initial base values */
    static const seq_t base_vals[6] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6, (seq_t)10
    };

    /* first call: allocate and initialise the cache up to index 5 */
    if (cache == NULL) {
        cache_cap = 64;                      /* start with room for 64 elems */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = base_vals[i];
        cache_n = 6;                        /* indices 0..5 are cached */
    }

    /* if n is already cached, just return it */
    if ((size_t)n < cache_n)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* reallocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* fill in any missing entries up to n */
    /* a(i) is defined for i >= 6 by the recurrence */
    for (size_t i = cache_n; i <= (size_t)n; ++i) {
        if (i < 6) {          /* safety: should not happen after cache_n>=6 */
            cache[i] = base_vals[i];
            continue;
        }
        /* recurrence: a(i) = 2*a(i-1)-2*a(i-3)+2*a(i-4)-a(i-6) */
        seq_t val = cache[i-1];
        val = val + val;          /* 2*a(i-1) */
        val = val - cache[i-3];
        val = val - cache[i-3];    /* -2*a(i-3) */
        seq_t tmp = cache[i-4];
        tmp = tmp + tmp;          /* 2*a(i-4) */
        val = val + tmp;
        val = val - cache[i-6];   /* -a(i-6) */
        cache[i] = val;
    }

    cache_n = (size_t)n + 1;      /* update cached range */
    return cache[(size_t)n];
}
