/*
 * A211582  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          three, four, six or seven distinct values for every i,j,k<=n.
 * https://oeis.org/A211582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence of order 10 a(n) = 2*a(n-1) + 13*a(n-2) - 26*a(n-3) -
 *            57*a(n-4) + 116*a(n-5) + 97*a(n-6) - 216*a(n-7) - 36*a(n-8) +
 *            144*a(n-9) - 36*a(n-10) with initial values a(1)…a(10) as given.
 *            This function memoises past results so any order of calls is
 *            supported
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static int   initialized = 0;

    /* first call: allocate and initialise first 10 terms */
    if (!initialized) {
        /* small initial capacity, grows later as needed */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initial terms a(1) … a(10) */
        static const seq_t init[10] = {16,44,116,284,696,1672,3996,9580,22656,54456};
        for (idx_t i = 0; i < 10; ++i) cache[i] = init[i];
        cache_len = 10;
        initialized = 1;
    }

    /* extend cache if requested n is beyond current length */
    if (cache_len < n) {
        /* ensure capacity */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 10) {
                /* first 10 have already been initialised */
                continue;
            } else {
                /* recurrence using indices relative to cache (0‑based) */
                seq_t pos = (seq_t)2  * cache[i-2]   /* a(i-1) */
                         + (seq_t)13 * cache[i-3]   /* a(i-2) */
                         + (seq_t)116 * cache[i-6]  /* a(i-5) */
                         + (seq_t)97  * cache[i-7]  /* a(i-6) */
                         + (seq_t)144 * cache[i-10];/* a(i-9) */
                seq_t neg = (seq_t)26  * cache[i-4]   /* a(i-3) */
                         + (seq_t)57  * cache[i-5]   /* a(i-4) */
                         + (seq_t)216 * cache[i-8]   /* a(i-7) */
                         + (seq_t)36  * cache[i-9]   /* a(i-8) */
                         + (seq_t)36  * cache[i-11]; /* a(i-10) */
                cache[i-1] = pos - neg; /* guaranteed non‑negative */
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
