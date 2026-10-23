/*
 * A165181  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 47*(a(n‑1)+…+a(n‑7)) – 1128*a(n‑8) + (n==8?1:0)
 * Initial values: a(0)=1, a(1)=49, a(2)=2352, a(3)=112896,
 *                 a(4)=5419008, a(5)=260112384, a(6)=12485394432,
 *                 a(7)=599298932736
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* hard‑coded first eight terms of the sequence */
    static const seq_t init[8] =
    {
        (seq_t)1ULL,
        (seq_t)49ULL,
        (seq_t)2352ULL,
        (seq_t)112896ULL,
        (seq_t)5419008ULL,
        (seq_t)260112384ULL,
        (seq_t)12485394432ULL,
        (seq_t)599298932736ULL
    };

    /* initialise the cache on the first call */
    if (cache == NULL)
    {
        cache_sz = 8;                      /* indices 0 … 7 are known */
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                /* out‑of‑memory – unlikely */
        for (idx_t i = 0; i < cache_sz; ++i)
            cache[i] = init[i];
    }

    /* request is already cached */
    if (n < cache_sz)
        return cache[n];

    /* extend the cache so that it contains indices 0 … n */
    const idx_t needed = n + 1;
    idx_t old_sz = cache_sz;

    if (needed > cache_sz)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;                /* allocation failure */
        cache    = new_cache;
        cache_sz = needed;                  /* update after reallocation */
    }

    /* coefficients of the recurrence */
    const seq_t c47   = (seq_t)47;
    const seq_t c1128 = (seq_t)1128;

    /* compute new terms from old_sz up to n (inclusive) */
    for (idx_t i = old_sz; i <= n; ++i)
    {
        seq_t term = c47 * cache[i-1];
        term += c47 * cache[i-2];
        term += c47 * cache[i-3];
        term += c47 * cache[i-4];
        term += c47 * cache[i-5];
        term += c47 * cache[i-6];
        term += c47 * cache[i-7];
        term -= c1128 * cache[i-8];
        if (i == 8)
            term += (seq_t)1;          /* numerator contribution for n==8 */
        cache[i] = term;
    }

    return cache[n];
}
