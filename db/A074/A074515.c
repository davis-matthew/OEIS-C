/*
 * A074515  a(n) = 1^n + 4^n + 9^n.
 * https://oeis.org/A074515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 4^n + 9^n Recurrence: a(0)=3, a(1)=14;
 *            a(n)=13*a(n-1)-36*a(n-2)+24 for n≥2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t    cache_len = 0;   /* number of elements stored  (>=2 after init) */
    static idx_t    cache_cap = 0;   /* capacity of the cache array      */

    /* initialise on first call */
    if (!cache)
    {
        cache_cap = 2;
        cache_len = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)3;   /* a(0) */
        cache[1] = (seq_t)14;  /* a(1) */
    }

    /* extend cache up to index n */
    while (cache_len <= n)
    {
        /* grow the cache if necessary */
        if (cache_len == cache_cap)
        {
            cache_cap *= 2;
            seq_t *newv = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!newv) exit(1);
            cache = newv;
        }

        /* compute a(cache_len) using the recurrence */
        idx_t i = cache_len;
        seq_t val = 13 * cache[i - 1] - 36 * cache[i - 2] + 24;
        cache[i] = val;
        cache_len++;
    }

    return cache[n];
}
