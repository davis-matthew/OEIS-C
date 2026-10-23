/*
 * A173711  Nonnegative integers, six even followed by two odd.
 * https://oeis.org/A173711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173711: Nonnegative integers, six even followed by two odd
   Formula: a(n) = a(n-1)+a(n-2)-a(n-3)-a(n-4)+a(n-5)+a(n-6)-a(n-7)
   Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1
   This implementation uses an on‑demand linear recurrence with
   a static cache to avoid recomputing terms for arbitrary call order.
*/

#include <stdlib.h>

static seq_t *cache          = NULL;   // cache of computed values
static size_t cache_cap      = 0;       // current allocation size
static size_t cache_len      = 0;       // number of values stored in cache

seq_t a(idx_t n)
{
    /* First nine values are given explicitly */
    static const seq_t init_vals[7] = {0,0,0,0,0,0,1};

    if (n < 7)          /* return one of the initial values */
        return init_vals[n];

    /* First time we need the cache: allocate and fill the first 7 values */
    if (cache_len == 0) {
        size_t need = (size_t)n + 1;
        if (need < 8) need = 8;          /* at least room for 0..7 */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = need;
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_len = 7;                  /* values 0..6 are ready */
    }

    /* If we already have a larger or equal index, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1)+a(i-2)-a(i-3)-a(i-4)+a(i-5)+a(i-6)-a(i-7) */
        seq_t val = cache[i-1] + cache[i-2] + cache[i-5] + cache[i-6];
        val -= cache[i-3];
        val -= cache[i-4];
        val -= cache[i-7];
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
