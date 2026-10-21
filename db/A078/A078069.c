/*
 * A078069  Expansion of (1-x)/(1+2*x+2*x^2).
 * https://oeis.org/A078069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*(a(n-1)+a(n-2)); a(0)=1, a(1)=-3
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 60, i64 n <= 124,
 *            i128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values up to cache_size-1 */
static idx_t cache_size = 0;  /* number of valid entries in cache */

/* Extend the cache to cover all indices up to n, computing
   new values using the recurrence.  This routine is safe to
   call with any n ≥ 0 and preserves previously computed terms. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return; /* already computed */

    /* Allocate or grow the cache */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) /* allocation failure – abort early */
        exit(EXIT_FAILURE);

    /* Initialisation: set the known base values if they are not yet set */
    if (cache_size == 0) {
        /* First time: a(0)=1 */
        cache[0] = (seq_t)1;
    }
    if (cache_size <= 1 && new_size > 1) {
        /* If a(1) is still missing, set it now */
        cache[1] = (seq_t)-3;
    }

    /* Compute new terms, starting from the first index that is still
       missing.  The recurrence only needs values for indices ≥ 2. */
    idx_t start = (cache_size > 1) ? cache_size : 2;
    for (idx_t i = start; i < new_size; ++i) {
        cache[i] = -( (cache[i - 1] + cache[i - 2]) * (seq_t)2 );
    }

    /* Update the cache size to reflect the maximum computed index. */
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
