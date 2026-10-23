/*
 * A033138  a(n) = floor(2^(n+2)/7).
 * https://oeis.org/A033138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n+2)/7). Recurrence: a(n) = a(n-3) + 2^(n-1) for
 *            n>3, with initial values a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array, a(n-1) stored at cache[n-1] */
static idx_t  cache_len = 0;     /* number of terms currently in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache array */
static seq_t  pow2_power;        /* 2^(cache_len-1) */

/* ensure cache contains values up to index n (1‑based) */
static void ensure_up_to(idx_t n)
{
    if (n <= cache_len)
        return;

    if (cache == NULL || cache_cap < n) {
        size_t new_cap = cache_cap;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first three terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)4;   /* a(3) */
        cache_len = 3;
        pow2_power = (seq_t)4; /* 2^(3-1) */
    }

    /* extend the cache to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        pow2_power = pow2_power * (seq_t)2;        /* 2^(k-1) */
        cache[cache_len] = cache[k - 4] + pow2_power; /* a(k) = a(k-3)+2^(k-1) */
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, undefined below */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            default: return (seq_t)4;   /* n==3 */
        }
    }
    ensure_up_to(n);
    return cache[n - 1];
}
