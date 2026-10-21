/*
 * A236048  Number of (n+1) X (1+1) 0..2 arrays colored with the difference of
 *          the maximum and minimum in each 2 X 2 subblock.
 * https://oeis.org/A236048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-2) + 4*a(n-3) + 24*a(n-4) + 12*a(n-5) + 8*a(n-6)
 *            for n > 8 Initial values: a(1)=81, a(2)=292, a(3)=1112,
 *            a(4)=3760, a(5)=13716, a(6)=46828, a(7)=169500, a(8)=582300
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* n is 1‑based; zero or negative values are undefined. */
    static seq_t *cache   = NULL;   /* stored sequence values */
    static size_t cache_len = 0;    /* number of terms stored (>=1) */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Lazy initialisation of the first eight terms. */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* one–based → zero‑based indices */
        cache[0] = (seq_t)81;     /* a(1) */
        cache[1] = (seq_t)292;    /* a(2) */
        cache[2] = (seq_t)1112;   /* a(3) */
        cache[3] = (seq_t)3760;   /* a(4) */
        cache[4] = (seq_t)13716;  /* a(5) */
        cache[5] = (seq_t)46828;  /* a(6) */
        cache[6] = (seq_t)169500; /* a(7) */
        cache[7] = (seq_t)582300; /* a(8) */
        cache_len = 8;
    }

    /* If the requested term is not yet computed, extend the cache. */
    if ((size_t)n > cache_len)
    {
        size_t new_cap = cache_cap;
        /* exponential growth until it can hold n terms */
        while (new_cap < (size_t)n)
            new_cap = new_cap * 2 + 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc may return NULL on failure; in that case keep previous pointer */
        if (new_cache == NULL)
            return (seq_t)0; /* unreachable in typical usage */
        cache = new_cache;
        cache_cap = new_cap;

        /* Compute terms from cache_len+1 up to n (both inclusive). */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* indices for a(i-k) correspond to cache[i-k-1] */
            seq_t val = (seq_t)9 * cache[(size_t)i - 3]   /* a(i-2) */
                      + (seq_t)4 * cache[(size_t)i - 4]   /* a(i-3) */
                      + (seq_t)24 * cache[(size_t)i - 5]  /* a(i-4) */
                      + (seq_t)12 * cache[(size_t)i - 6]  /* a(i-5) */
                      + (seq_t)8  * cache[(size_t)i - 7]; /* a(i-6) */
            cache[(size_t)i - 1] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
