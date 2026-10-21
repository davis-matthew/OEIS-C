/*
 * A219721  Expansion of (1+7*x+5*x^2+7*x^3+x^4)/(1-x-x^4+x^5).
 * https://oeis.org/A219721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=13, a(3)=20, a(4)=22; a(n) = a(n-1) +
 *            a(n-4) - a(n-5) for all n ≥ 5
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* allocated cache for computed values */
static size_t cache_len = 0;     /* number of values currently stored (indices 0 .. cache_len-1) */
static size_t cache_cap = 0;     /* capacity of cache array */

/* Expand the cache so it can hold at least indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                 /* already fits */

    /* double capacity until we reach at least n+1 */
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failed */

    if (cache)
    {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ensure we have computed up to n */
    ensure_cache(n);

    if (cache_len <= n)
    {
        /* initialise any missing entries, starting from where we left off */
        idx_t i = cache_len;

        /* pre‑defined initial values for i = 0 … 4 */
        for (; i <= 4 && i <= n; ++i)
        {
            switch (i)
            {
                case 0: cache[i] = 1; break;
                case 1: cache[i] = 8; break;
                case 2: cache[i] = 13; break;
                case 3: cache[i] = 20; break;
                case 4: cache[i] = 22; break;
            }
        }

        /* compute subsequent values using the recurrence */
        for (; i <= n; ++i)
        {
            /* a(i) = a(i-1) + a(i-4) - a(i-5) */
            seq_t v = cache[i-1] + cache[i-4];
            v -= cache[i-5];
            cache[i] = v;
        }

        cache_len = n + 1;   /* now we have values up to n */
    }

    return cache[n];
}
