/*
 * A078913  a(1)=a(2)=1, a(n)=a(n-1)+a(n-2) if n is not congruent to 3,
 *          a(n)=a(n-1)+a(n/3) if n is congruent to 3.
 * https://oeis.org/A078913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; a(n)=a(n-1)+a(n-2) if n%3!=0; a(n)=a(n-1)+a(n/3) if
 *            n%3==0
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies supplied by the build */
#include <stdlib.h>   /* for malloc, realloc, abort */

/* Cache for memoisation */
static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1), etc. */
static idx_t cache_size = 0;  /* current allocated size (number of slots) */

static void ensure_cache(idx_t n)
{
    /* Return if cache already holds indices up to n */
    if (n + 1 <= cache_size)
        return;

    /* Remember old size for initialisation loop */
    size_t old_size = (size_t)cache_size;
    size_t new_size = (size_t)(n + 1);

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Out‑of‑memory: abort the program */
        abort();
    }
    /* Initialise new entries to 0 (uncomputed) */
    for (size_t i = old_size; i < new_size; ++i)
        tmp[i] = (seq_t)0;

    cache = tmp;
    cache_size = new_size;

    /* Set the known base values once (they never change) */
    if (cache_size > 1) cache[1] = (seq_t)1;
    if (cache_size > 2) cache[2] = (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n<1 */

    ensure_cache(n);

    /* If already computed, return immediately */
    if (cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    /* Compute missing values from the smallest not yet computed index */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[(size_t)i] == (seq_t)0) {
            if ((i % 3) == 0) {
                /* a(n) = a(n-1) + a(n/3) when n%3==0 */
                cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i / 3)];
            } else {
                /* a(n) = a(n-1) + a(n-2) otherwise */
                cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
            }
        }
    }

    return cache[(size_t)n];
}
