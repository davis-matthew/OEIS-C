/*
 * A190870  a(n) = 11*a(n-1) - 22*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A190870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 22*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding computed terms */
static idx_t cache_size = 0;   /* allocated size of the array   */
static idx_t cache_max  = 0;   /* largest index that has been computed */

/* Ensure the cache is at least (n+1) elements long, reallocating if necessary */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
    if (!new_cache) {
        /* Out of memory – terminate (should not happen for the sizes expected) */
        abort();
    }

    /* Copy old data to the new buffer */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Zero the new part – values will be set later */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache     = new_cache;
    cache_size= new_size;
}

/* Return the nth term of A190870.  The function is thread‑safe for
 * sequential (single‑threaded) calls and memoises results. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    ensure_cache_size(n);

    /* Initialise base cases once */
    if (cache_max < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* If the requested term has already been computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev    = cache[i - 1];
        seq_t prevprev= cache[i - 2];
        cache[i] = prev * ((seq_t)11) - prevprev * ((seq_t)22);
    }
    cache_max = n;
    return cache[n];
}
