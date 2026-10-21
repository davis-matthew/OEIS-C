/*
 * A098575  a(n) = Sum_{k=0..floor(n/4)} C(n-2*k,2*k)*2^k.
 * https://oeis.org/A098575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} C(n-2*k,2*k)*2^k Recurrence: a(n) =
 *            2*a(n-1) – a(n-2) + 2*a(n-4) Initial values:
 *            a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 78, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of a(k) */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static size_t cache_len = 0;  /* number of values currently stored */

seq_t a(idx_t n)
{
    /* initialise cache with first four values */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = 1;
        cache_len = 4;
    }

    /* return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* grow cache as needed while computing values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {

        /* ensure there is enough capacity for cache[i] */
        if ((size_t)i + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while ((size_t)i + 1 > new_cap)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        seq_t val;
        if (i < 4)        /* base values */
            val = 1;
        else {            /* recurrence: 2*a[i-1] – a[i-2] + 2*a[i-4] */
            val = cache[i - 1];
            val += cache[i - 1];        /* 2*a[i-1] */
            val -= cache[i - 2];        /* –a[i-2]   */
            seq_t two_a4 = cache[i - 4];
            two_a4 += two_a4;           /* 2*a[i-4]  */
            val += two_a4;              /* +2*a[i-4] */
        }

        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
