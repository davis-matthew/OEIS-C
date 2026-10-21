/*
 * A077847  Expansion of (1-x)^(-1)/(1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A077847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) + 2*a(n-4); Initial values: a(0)=1,
 *            a(1)=3, a(2)=9, a(3)=23
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len  = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold at least n+1 terms. */
    if (cache_len == 0 || cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;        /* grow by powers of two */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first four terms when the cache is created. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)23;
        cache_len = 4;
    }

    if ((idx_t)n < cache_len)
        return cache[n];

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = (seq_t)3 * cache[i - 1];
        seq_t t2 = (seq_t)4 * cache[i - 3];
        seq_t t3 = (seq_t)2 * cache[i - 4];
        cache[i] = t1 - t2 + t3;
    }
    cache_len = n + 1;
    return cache[n];
}
