/*
 * A178577  Partial sums of round(5^n/9).
 * https://oeis.org/A178577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = a(n-6) + 434 * 5^(n-5) for n > 5 Initial values: a(0)=0,
 *            a(1)=1, a(2)=4, a(3)=18, a(4)=87, a(5)=434
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {0, 1, 4, 18, 87, 434};

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of computed terms in cache */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* Allocate and initialize cache */
static void init_cache(void)
{
    size_t cap = 16;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) exit(1);
    for (size_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
    cache_size = 6;
    cache_cap  = cap;
}

/* Ensure cache can hold terms up to index n */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n + 1) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 6) return init_vals[n];

    if (!cache) init_cache();
    ensure_capacity(n);

    /* Compute missing terms using the 5‑term linear recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t term;
#ifdef __cplusplus
        term = seq_t(6) * cache[idx-1]
             - seq_t(5) * cache[idx-2]
             - cache[idx-3]
             + seq_t(6) * cache[idx-4]
             - seq_t(5) * cache[idx-5];
#else
        unsigned __int128 t = (unsigned __int128)cache[idx-1] * 6
                           - (unsigned __int128)cache[idx-2] * 5
                           - (unsigned __int128)cache[idx-3]
                           + (unsigned __int128)cache[idx-4] * 6
                           - (unsigned __int128)cache[idx-5] * 5;
        term = (seq_t)t;
#endif
        cache[idx] = term;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
