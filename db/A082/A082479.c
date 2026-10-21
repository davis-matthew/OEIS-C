/*
 * A082479  a(1)=1, a(n)=a(floor(n/2))+a(floor(n/3)).
 * https://oeis.org/A082479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(n/2)) + a(floor(n/3)) with a(1) = 1. * For the
 *            recurrence to work when n = 2, we also define a(0) = 1
 *            internally * (though a(0) is never returned directly)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Make sure the cache is large enough to index n. */
static void
ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;                     /* already large enough */

    idx_t new_size = n + 1;          /* need indices 0..n inclusive   */
    seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    if (!new_cache)                 /* out‑of‑memory; in production abort */
        return;

    /* Copy any existing values. */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Initialise new slots to zero. */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = new_size;

    /* Base values for the recurrence. */
    cache[0] = (seq_t)1;
    if (cache_size > 1)
        cache[1] = (seq_t)1;
}

/* Public function to compute a(n). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* internal use only */

    ensure_cache(n);

    if (cache[n] != (seq_t)0)
        return cache[n];

    seq_t left  = a(n / 2);
    seq_t right = a(n / 3);
    cache[n] = left + right;
    return cache[n];
}
