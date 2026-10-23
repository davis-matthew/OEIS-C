/*
 * A195619  Denominators of Pythagorean approximations to 4.
 * https://oeis.org/A195619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 65*a(n-1) + 65*a(n-2) - a(n-3) with initial values
 *            a(1)=16, a(2)=1040, a(3)=68640
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to allocate an array of seq_t, compatible with C and C++ builds. */
static seq_t* allocate_array(idx_t cap)
{
#ifdef __cplusplus
    return new seq_t[(size_t)cap];
#else
    return (seq_t*)malloc((size_t)cap * sizeof(seq_t));
#endif
}

/* Helper to free an array of seq_t, compatible with C and C++ builds. */
static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, but safe fallback */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialisation with the first three terms */
    if (cache == NULL) {
        cache_cap = 4;                    /* at least capacity for 3 terms */
        cache = allocate_array(cache_cap);
        if (!cache)
            abort();                     /* allocation failed */
        cache[0] = (seq_t)16;             /* a(1) */
        cache[1] = (seq_t)1040;           /* a(2) */
        cache[2] = (seq_t)68640;          /* a(3) */
        cache_len = 3;
    }

    /* Grow the cache if needed */
    if (n > cache_len) {
        /* Ensure enough capacity */
        if (cache_cap < n) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap < n)
                new_cap = n;
            seq_t *new_cache = allocate_array(new_cap);
            if (!new_cache)
                abort();                 /* allocation failed */
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_array(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t val = (seq_t)65 * cache[i-1] + (seq_t)65 * cache[i-2] - cache[i-3];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n-1]; /* cache is zero‑indexed, n starts at 1 */
}
