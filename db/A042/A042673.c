/*
 * A042673  Denominators of continued fraction convergents to sqrt(866).
 * https://oeis.org/A042673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 84870*a(n-8) - a(n-16) Initial values a(0)…a(15) are as
 *            listed (see OEIS entry)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* initial 16 terms of the sequence */
static const seq_t init[16] = {
    (seq_t)1,      (seq_t)2,      (seq_t)5,      (seq_t)7,
    (seq_t)201,    (seq_t)208,    (seq_t)617,    (seq_t)1442,
    (seq_t)84253,  (seq_t)169948, (seq_t)424149, (seq_t)594097,
    (seq_t)17058865,(seq_t)17652962,(seq_t)52364789,(seq_t)122382540
};

/* cache of previously computed terms */
static seq_t *cache   = NULL;   /* pointer to cached array */
static size_t cache_cap = 0;    /* allocated capacity (number of slots) */
static size_t cache_len = 0;    /* number of terms already stored (0‑based+1) */

/* ensure that the cache contains at least the first (n+1) terms */
static void ensure_cache(idx_t n)
{
    /* if the cache already covers the requested index, nothing to do */
    if (cache_len > (size_t)n)
        return;

    /* if the cache is empty, initialise it with the first 16 terms */
    if (cache_len == 0) {
        size_t need_cap = 16;
        if (n + 1 > need_cap)
            need_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(NULL, need_cap * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – abort further work */
            return;
        }
        cache_cap = need_cap;
        for (size_t i = 0; i < sizeof(init)/sizeof(init[0]); ++i)
            cache[i] = init[i];
        cache_len = sizeof(init)/sizeof(init[0]);
        if (n + 1 <= cache_len)
            return;
    } else if (cache_cap <= (size_t)n) {
        /* enlarge the cache to accommodate index n */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * cache_cap)
            new_cap = 2 * cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort further work */
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 16) {
            /* the first 16 entries were already filled from init */
            continue;
        }
        /* a(i) = 84870*a(i-8) - a(i-16) */
        seq_t prod = (seq_t)84870 * cache[i - 8];
        cache[i] = prod - cache[i - 16];
    }
    if (cache_len <= (size_t)n)
        cache_len = (size_t)n + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache_len <= (size_t)n)
        return (seq_t)0;          /* allocation failure or logic error */
    return cache[n];
}
