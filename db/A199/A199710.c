/*
 * A199710  Expansion of (1+x-14*x^2+13*x^3)/(1-28*x^2+169*x^4).
 * https://oeis.org/A199710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-2) - 169*a(n-4) Initial values: a(0)=1, a(1)=1,
 *            a(2)=14, a(3)=41
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache of computed terms */
static idx_t cache_len = 0;       /* number of terms stored */
static idx_t cache_cap = 0;       /* allocated capacity */

/* Ensure the cache can hold indices up to `n`. */
static void cache_ensure(idx_t n)
{
    if (cache_cap <= n)
    {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out-of-memory: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialisation: set first four terms */
    if (cache_len == 0)
    {
        cache_ensure(3);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 14;
        cache[3] = 41;
        cache_len = 4;
    }

    if (n < cache_len) return cache[n];

    /* Grow cache and compute missing terms up to n */
    cache_ensure(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 4) continue;            /* already initialised */
        seq_t prod1 = cache[i - 2] * (seq_t)28;
        seq_t prod2 = cache[i - 4] * (seq_t)169;
        cache[i] = prod1 - prod2;
    }
    cache_len = n + 1;
    return cache[n];
}
