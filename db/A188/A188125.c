/*
 * A188125  Number of strictly increasing arrangements of 6 nonzero numbers in
 *          -(n+4)..(n+4) with sum zero.
 * https://oeis.org/A188125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 174, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=2*a(n-1)-a(n-3)-a(n-5)+2*a(n-8)-a(n-11)-a(n-13)+2*a(n-15)-a(n-16)
   with initial values a(0)..a(15) computed by brute force. */

#include <stddef.h>
#include <stdlib.h>

/* Brute‑force enumeration for n ≤ 15. */
static seq_t brute_count(idx_t n)
{
    /* The range is -(n+4)..-(1) and 1..(n+4).  Length is 2*(n+4). */
    int m = (int)n + 4;
    int len = 2 * m;
    int values[40];                     /* len ≤ 38 for n ≤ 15 */
    int idx = 0;
    for (int k = -m; k <= -1; ++k) values[idx++] = k;
    for (int k = 1;  k <= m;  ++k) values[idx++] = k;

    seq_t cnt = 0;
    for (idx_t i = 0; i + 5 < len; ++i)
        for (idx_t j = i + 1; j + 4 < len; ++j)
            for (idx_t k = j + 1; k + 3 < len; ++k)
                for (idx_t l = k + 1; l + 2 < len; ++l)
                    for (idx_t p = l + 1; p + 1 < len; ++p)
                        for (idx_t q = p + 1; q < len; ++q)
                        {
                            int sum = values[i] + values[j] + values[k]
                                      + values[l] + values[p] + values[q];
                            if (sum == 0) ++cnt;
                        }
    return cnt;
}

/* Public function. */
seq_t a(idx_t n)
{
    /* Static cache to store already computed values. */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;   /* number of elements stored */

    /* Pre‑compute the first 16 values (a(0) … a(15)). */
    static seq_t base[16];
    static int  base_done = 0;
    if (!base_done)
    {
        for (idx_t i = 0; i < 16; ++i) base[i] = brute_count(i);
        base_done = 1;
    }

    /* Ensure the cache can hold the first 16 values. */
    if (cache_sz < 16)
    {
        size_t new_sz = 16;
        seq_t *tmp    = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache    = tmp;
        for (size_t i = 0; i < 16; ++i) cache[i] = base[i];
        cache_sz = new_sz;
    }

    /* Extend the cache up to n if necessary. */
    if ((size_t)n + 1 > cache_sz)
    {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp    = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_sz; i <= (size_t)n; ++i)
        {
            /* recurrence valid for i ≥ 16; indices in range for i≥16. */
            seq_t val = cache[i-1] * 2;
            val -= cache[i-3];
            val -= cache[i-5];
            val += cache[i-8] * 2;
            val -= cache[i-11];
            val -= cache[i-13];
            val += cache[i-15] * 2;
            val -= cache[i-16];
            cache[i] = val;
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
