/*
 * A062883  (1-2*cos(1/11*Pi))^n+(1+2*cos(2/11*Pi))^n+(1-2*cos(3/11*Pi))^n+(1+2*cos(4/11*Pi))^n+(1-2*cos(5/11*Pi))^n.
 * https://oeis.org/A062883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)-2*a(n-2)-5*a(n-3)+2*a(n-4)+a(n-5) for n>5, with
 *            a(1)=4, a(2)=12, a(3)=25, a(4)=64, a(5)=159
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache   = NULL;   /* 1‑based cache for a(n) */
static size_t cache_len = 0;    /* largest n already computed (>=0) */
static size_t cache_cap = 0;    /* allocated length of cache (>= cache_len+1) */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 10;                /* extra space */
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset starts at 1 – undefined for n==0 */

    if (cache == NULL) {
        cache_cap   = 6;                 /* indices 0 … 5 */
        cache       = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)64;
        cache[5] = (seq_t)159;
        cache_len  = 5;
    }

    if ((size_t)n <= cache_len)
        return cache[n];

    ensure_cache_capacity(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1]
                 - (seq_t)2 * cache[i-2]
                 - (seq_t)5 * cache[i-3]
                 + (seq_t)2 * cache[i-4]
                 + cache[i-5];
    }
    cache_len = (size_t)n;
    return cache[n];
}
