/*
 * A175782  Expansion of 1/(1 - x - x^20 - x^39 + x^40).
 * https://oeis.org/A175782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-20) + a(n-39) - a(n-40), with a(0) … a(19) =
 *            1. All other a(n) are obtained by the recurrence. This
 *            implementation memoises the values in a static array so that
 *            calls may occur in arbitrary order
 * Range:     u8 n <= 59, u16 n <= 108, u32 n <= 206, u64 n <= 403,
 *            u128 n <= 797, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Allocate or grow the cache so that cache[0..n] exists.
   Initialise the first 20 entries with the constant value 1
   only once. */
static seq_t *cache      = NULL;   /* array of computed terms */
static idx_t  cache_len  = 0;       /* current allocated size */
static idx_t  max_calc   = 0;       /* highest index computed */
static bool   init_done  = false;   /* first-initialisation flag */

static void ensure_cache(idx_t n)
{
    /* Grow the cache to at least n+1 elements if needed. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: abort – the calling harness
               will not recover from this. */
            abort();
        }
        cache = tmp;

        /* Zero initialises any newly added slots. */
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;

        cache_len = new_len;
    }

    /* Initialise the fixed prefix a[0] .. a[19] = 1
       only the first time we have at least 20 slots. */
    if (!init_done && cache_len >= 20) {
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = 1;
        init_done = true;
        /* The highest pre‑computed value after this point is 19. */
        max_calc = 19;
    }
    else if (!init_done) {
        /* If still less than 20 slots we simply set the lower
           entries to 1 as and when they are allocated. */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = 1;
        init_done = true;
        max_calc = cache_len - 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 20) {
        return 1; /* Base values for n = 0 … 19. */
    }

    ensure_cache(n);

    if (max_calc >= n)    /* Value already computed. */
        return cache[n];

    /* Extend the cache up to n using the recurrence. */
    for (idx_t i = max_calc + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 20];
        if (i >= 39)
            val += cache[i - 39];
        if (i >= 40)
            val -= cache[i - 40];
        cache[i] = val;
    }

    max_calc = n;
    return cache[n];
}
