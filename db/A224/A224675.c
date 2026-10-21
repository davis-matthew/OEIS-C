/*
 * A224675  Number of (n+1) X 8 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n>6
   initial values:
   a(1)=308, a(2)=470, a(3)=617, a(4)=864, a(5)=1166, a(6)=1608 */

#include <stdlib.h>

static const seq_t init_vals[6] = {
    (seq_t)308, (seq_t)470, (seq_t)617,
    (seq_t)864, (seq_t)1166, (seq_t)1608
};

static seq_t *cache      = NULL;   /* storage for computed terms */
static size_t cache_cap  = 0;       /* allocated capacity (bytes) */
static size_t cache_len  = 0;       /* largest index that has been computed */

/* ensure cache has space for indices 0..needed */
static void ensure_cache(idx_t needed)
{
    if (cache_cap > (size_t)needed + 1) return;     /* already enough */

    size_t new_cap = (size_t)needed + 1;
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2; /* grow fast */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                      /* allocation failed – keep old */

    /* zero‑initialize the new entries */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but harmless */

    if (n <= 6) return init_vals[n - 1];   /* return one of the initial values */

    /* initialise the cache on the first call */
    if (!cache) {
        size_t init_cap = (size_t)(n >= 6 ? n + 1 : 7);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */

        for (size_t i = 0; i < init_cap; ++i)
            cache[i] = (seq_t)0;

        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 6; ++i)
            cache[(size_t)i] = init_vals[i - 1];

        cache_cap = init_cap;
        cache_len = 6;                     /* we have computed up to a(6) */
    } else {
        ensure_cache(n);                   /* make sure we have enough room */
    }

    if (n <= cache_len) return cache[(size_t)n]; /* already computed */

    /* compute terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t v = 4 * cache[(size_t)(k - 1)]
                - 5 * cache[(size_t)(k - 2)]
                +   cache[(size_t)(k - 3)]
                + 2 * cache[(size_t)(k - 4)]
                -   cache[(size_t)(k - 5)];
        cache[(size_t)k] = v;
    }
    cache_len = n;

    return cache[(size_t)n];
}
