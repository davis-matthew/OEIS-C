/*
 * A210460  Expansion of x*(1+x)/(1-x-2*x^2-2*x^3-x^4).
 * https://oeis.org/A210460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)+2*a(n-3)+a(n-4) for n>=5, with a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=10
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i+1) (i.e. a at index i+1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already computed (>=0) */
    static size_t cache_cap = 0;   /* allocated size of the cache array */

    /* Ensure the cache has room for n terms. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* abort on memory error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill the cache up to term n. */
    if (cache_len < n) {
        /* Predefined first four terms. */
        static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)10 };

        /* Add missing initial terms. */
        for (idx_t k = cache_len + 1; k <= n && k <= 4; ++k)
            cache[k - 1] = init[k - 1];

        /* Compute remaining terms using the linear recurrence. */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t val;
            if (k == 1)
                val = (seq_t)1;
            else if (k == 2)
                val = (seq_t)2;
            else if (k == 3)
                val = (seq_t)4;
            else if (k == 4)
                val = (seq_t)10;
            else {
                val = cache[k - 2];                  /* a(k-1) */
                val += cache[k - 3] + cache[k - 3];  /* 2*a(k-2) */
                val += cache[k - 4] + cache[k - 4];  /* 2*a(k-3) */
                val += cache[k - 5];                /* a(k-4) */
            }
            cache[k - 1] = val;
        }

        cache_len = n;
    }

    return cache[n - 1];
}
