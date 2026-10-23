/*
 * A181343  a(n) = (35*n^4 - 35*n^3 + 55*n^2 - 25*n + 6)/6.
 * https://oeis.org/A181343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 164, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181343
   Formula: a(n) = (35*n^4 - 35*n^3 + 55*n^2 - 25*n + 6) / 6
   Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + 140
     (n > 4;  a(1)=6, a(2)=76, a(3)=386, a(4)=1251) */

/* -------------------------- dependencies -------------------------- */
#include <stdlib.h>   /* malloc, free, abort */

/* -------------------------- internal state -------------------------- */
static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
static idx_t cache_limit = 0; /* highest index currently allocated */

/* Allocate or enlarge the cache to hold at least `needed` indices
   (1‑based).  All new entries are zero‑initialised, then the first
   four terms are written if the corresponding slot is still zero. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_limit) return;

    /* Choose a new size: double the old size or at least `needed`. */
    idx_t new_limit = cache_limit > 0 ? cache_limit * 2 : 8;
    if (new_limit < needed) new_limit = needed;

    /* Allocate a fresh array of seq_t. */
    seq_t *new_cache = (seq_t*)malloc((new_limit + 1) * sizeof(seq_t));
    if (!new_cache) abort();

    /* Initialise new slots to zero. */
    for (idx_t i = 0; i <= new_limit; ++i) new_cache[i] = 0;

    /* Copy any previously cached values. */
    for (idx_t i = 1; i <= cache_limit; ++i) new_cache[i] = cache[i];

    /* Write initial values where missing. */
    if (!new_cache[1]) new_cache[1] = (seq_t)6;
    if (!new_cache[2]) new_cache[2] = (seq_t)76;
    if (!new_cache[3]) new_cache[3] = (seq_t)386;
    if (!new_cache[4]) new_cache[4] = (seq_t)1251;

    /* Replace old cache. */
    free(cache);
    cache      = new_cache;
    cache_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Public function                                                      */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* Ensure the cache is large enough. */
    if (n > cache_limit) grow_cache(n);

    /* Return a cached value if already known. */
    if (cache[n] != 0) return cache[n];

    /* Compute missing terms sequentially up to n. */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t val = cache[i - 1] * 4;
        val -= cache[i - 2] * 6;
        val += cache[i - 3] * 4;
        val -= cache[i - 4];
        val += 140;
        cache[i] = val;
    }

    return cache[n];
}
