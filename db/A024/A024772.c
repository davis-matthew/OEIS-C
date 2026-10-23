/*
 * A024772  Expansion of 1/((1-x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A024772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=29, a(2)=554, a(3)=8794;
   for n≥4: a(n)=29·a(n-1)−287·a(n-2)+1051·a(n-3)−792·a(n-4) */
#include <stddef.h>
#include <stdlib.h>

/* Static cache of computed values.  */
static seq_t *cache = NULL;        /* array of computed terms   */
static size_t cache_cap = 0;       /* allocated size (≥ max index+1) */
static idx_t max_computed = 0;     /* number of elements already filled */

/* Grow the cache to accommodate indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if (cache_cap <= (size_t)need) {
        size_t new_cap = (size_t)need + 1;
        if (new_cap < 4u) new_cap = 4u;             /* at least 4 slots */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the first four terms. */
static void init_cache(void)
{
    ensure_capacity(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)554;
    cache[3] = (seq_t)8794;
    max_computed = 4;
}

/* Compute the n‑th term of A024772. */
seq_t a(idx_t n)
{
    /* Lazily initialise the cache on first call */
    if (cache == NULL) {
        init_cache();
    }

    /* If the value is already known, return it immediately */
    if (n < max_computed) {
        return cache[n];
    }

    /* Ensure the cache can hold all terms up to 'n' */
    ensure_capacity(n);

    /* Compute missing terms from the first unknown index */
    for (idx_t i = max_computed; i <= n; ++i) {
        cache[i] =
            (seq_t)29  * cache[i-1]
          - (seq_t)287 * cache[i-2]
          + (seq_t)1051 * cache[i-3]
          - (seq_t)792  * cache[i-4];
    }

    max_computed = n + 1;
    return cache[n];
}
