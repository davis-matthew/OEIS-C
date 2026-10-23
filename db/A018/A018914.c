/*
 * A018914  Pisot sequence T(2,5), a(n) = floor(a(n-1)^2/a(n-2)).
 * https://oeis.org/A018914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)^2 / a(n-2)), a(0) = 2, a(1) = 5 (Pisot
 *            sequence T(2,5), OEIS A018914)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;   /* array holding a(i) */
static size_t cache_cap = 0;    /* capacity of cache (in seq_t units) */
static size_t cache_len = 0;    /* number of valid entries in cache */

/* Ensure the cache contains a(0)…a(n).  Earlier entries are preserved. */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache if necessary. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Zero the newly added part. */
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First two terms are known.  Initialise them if missing. */
    if (cache_len < 2) {
        if (cache_len < 1) cache[0] = (seq_t)2;
        if (cache_len < 2) cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev      = cache[i-1];
        seq_t prevprev  = cache[i-2];

        /* For small integer types (< 9 bytes) use 128‑bit arithmetic
           to avoid overflow on intermediate products.  For larger
           seq_t (arbitrary‑precision) just use the natural operators. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)prev * (unsigned __int128)prev;
            cache[i] = (seq_t)(prod / (unsigned __int128)prevprev);
        } else {
            cache[i] = prev * prev / prevprev;
        }
    }

    /* Record the number of valid entries. */
    if (cache_len < (size_t)n + 1) {
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
