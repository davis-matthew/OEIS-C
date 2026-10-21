/*
 * A218730  a(n) = (27^n - 1)/26.
 * https://oeis.org/A218730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 28*a(n-1) - 27*a(n-2) (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values of a(n) */
static idx_t cache_size = 0;     /* number of cached entries */

/* Ensure that cache[n] is computed for all n <= req.   */
static void ensure_cache(idx_t req)
{
    if (req < cache_size)
        return;                     /* already available */

    size_t new_size = (size_t)(req + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)                /* memory allocation failed */
        return;

    /* Initialise base cases if needed */
    if (cache_size == 0) {
        if (new_size > 0)
            new_cache[0] = (seq_t)0;
        if (new_size > 1)
            new_cache[1] = (seq_t)1;
    } else if (cache_size == 1) {
        new_cache[0] = cache[0];
        new_cache[1] = (seq_t)1;
    }
    /* For cache_size >= 2 the first two entries are already
       preserved by realloc; no need to touch them.   */

    /* Compute new entries */
    idx_t start = (cache_size >= 2) ? cache_size : (idx_t)2;
    for (idx_t i = start; i <= req; ++i) {
        new_cache[i] = 28 * new_cache[i - 1] - 27 * new_cache[i - 2];
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
