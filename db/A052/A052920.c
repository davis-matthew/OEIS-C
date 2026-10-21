/*
 * A052920  a(n) = a(n-3) + a(n-5) with initial values 1,0,0,1,0.
 * https://oeis.org/A052920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + a(n-5), a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0
 * Range:     u8 n <= 39, u16 n <= 70, u32 n <= 132, u64 n <= 257,
 *            u128 n <= 508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of initialized elements */
    static size_t cache_cap = 0;   /* allocated capacity */
    static int initialized = 0;    /* 0 = not yet set up */

    /* one‑time initialization of the five seed values */
    if (!initialized)
    {
        cache_cap = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        cache_len = 5;
        initialized = 1;
    }

    /* size_t version of the index */
    size_t idx = (size_t)n;

    /* ensure the cache can address index n */
    if (idx >= cache_len)
    {
        /* grow cache if necessary */
        if (cache_cap <= idx)
        {
            size_t new_cap = cache_cap ? cache_cap : 5;
            while (new_cap <= idx) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;         /* allocation failure */
            /* initialise new slots to zero */
            memset(tmp + cache_len, 0, (new_cap - cache_len) * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms up to n */
        for (size_t i = cache_len; i <= idx; ++i)
        {
            seq_t val = 0;
            if (i >= 3) val += cache[i - 3];
            if (i >= 5) val += cache[i - 5];
            cache[i] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
