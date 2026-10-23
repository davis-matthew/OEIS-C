/*
 * A189667  Partial sums of A189664.
 * https://oeis.org/A189667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   s(n) = floor((n+1)/3) – s(floor((n+1)/3)) + s(floor(n/3)), s(0)
 *            = 0 (partial sums of A189664)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

/* Static cache for memoised values */
static seq_t *cache = NULL;         /* array of computed s(i) */
static idx_t  cache_size = 0;       /* current allocated size (≥ computed_max+1) */
static idx_t  computed_max = 0;     /* largest index already computed */

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                      /* already large enough */

    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_cache(new_size);   /* zero‑initialised */
    if (!new_cache)
        exit(EXIT_FAILURE);          /* allocation failed */

    /* copy old cached values */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old cache if it exists */
    if (cache)
        free_cache(cache);

    cache       = new_cache;
    cache_size  = new_size;
}

/* Return the n‑th term of the sequence (a(1) = 0, a(2) = 1, ...) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n==0, but defined safely */

    ensure_cache(n);
    cache[0] = 0;                   /* guarantee s(0)=0 */

    /* compute all values from (computed_max+1) up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        idx_t up   = (i + 1) / 3;    /* floor((i+1)/3) */
        idx_t down = i / 3;          /* floor(i/3) */
        cache[i]   = (seq_t)up - cache[up] + cache[down];
    }

    computed_max = n;
    return cache[n];
}
