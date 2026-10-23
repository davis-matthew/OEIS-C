/*
 * A164581  a(n) = 5*a(n - 1) + a(n - 2), with a(0)=1, a(1)=2.
 * https://oeis.org/A164581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2); a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* Cached sequence values */
static idx_t  stored = 0;        /* Number of entries currently cached */
static idx_t  cap    = 0;        /* Capacity of the cache array */

/* Ensure the cache can hold all indices up to 'up_to' */
static void ensure_cache(idx_t up_to)
{
    if (cap <= up_to)
    {
        idx_t new_cap = up_to + 1;
        seq_t *new_cache;
        if (cache)
        {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
        }
        else
        {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cap   = new_cap;
    }
}

/* Compute the n-th term of OEIS sequence A164581 */
seq_t a(idx_t n)
{
    /* Initialize the first two terms if they are not yet cached */
    if (stored < 2)
    {
        ensure_cache(1);            /* need space for indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        stored   = 2;
    }

    /* Return immediately if the requested value is already cached */
    if (n < stored)
        return cache[n];

    /* Extend the cache to accommodate all indices up to n */
    ensure_cache(n);

    /* Compute successive terms using the recurrence */
    for (idx_t i = stored; i <= n; ++i)
        cache[i] = ((seq_t)5) * cache[i - 1] + cache[i - 2];

    /* Mark the newly computed range as cached */
    stored = n + 1;
    return cache[n];
}
