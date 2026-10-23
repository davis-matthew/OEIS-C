/*
 * A116413  Expansion of (1+x)/(1-2x-x^2-x^3).
 * https://oeis.org/A116413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) Initial values: a(0) = 1, a(1)
 *            = 3, a(2) = 7
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Persistent cache for computed values */
static seq_t *cache = NULL;      /* dynamic array of a(i) values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_computed = (idx_t)-1;  /* largest n for which cache[i] is valid */

/* Ensure cache has space for at least indices 0..n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = n + 1;            /* exact size needed */
    seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) exit(1);           /* allocation failed */

    if (cache) {
        memcpy(new_cache, cache, sizeof(seq_t) * cache_cap);
        free(cache);
    }
    /* zero‑initialize new slots */
    if (new_cap > cache_cap)
        memset(new_cache + cache_cap, 0, sizeof(seq_t) * (new_cap - cache_cap));

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* First time: allocate and initialise base values */
    if (cache == NULL) {
        idx_t first_cap = (n < 2) ? n + 1 : 3;   /* at least 3 for a(0..2) */
        ensure_capacity(first_cap - 1);
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)3;
        if (cache_cap > 2) cache[2] = (seq_t)7;
        last_computed = (cache_cap > 2) ? 2 : (cache_cap - 1);
    } else {
        /* Expand cache if needed */
        if (n >= cache_cap)
            ensure_capacity(n);

        /* Initialise base values if cache was resized to small size */
        if (last_computed < 2) {
            cache[0] = (seq_t)1;
            if (cache_cap > 1) cache[1] = (seq_t)3;
            if (cache_cap > 2) cache[2] = (seq_t)7;
            last_computed = (cache_cap > 2) ? 2 : (cache_cap - 1);
        }
    }

    /* Compute missing values up to n using recurrence */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1] + cache[i-2] + cache[i-3];
        }
        last_computed = n;
    }

    return cache[n];
}
