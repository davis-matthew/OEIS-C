/*
 * A082030  Expansion of e.g.f. exp(x)/(1-x)^3.
 * https://oeis.org/A082030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(n) = (n+3)*a(n-1) - (n-1)*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoized values */
static seq_t *cache         = NULL;   /* cache[i] = a(i) */
static idx_t cache_size     = 0;     /* number of cached elements */

/* grow the cache so that it can hold values up to index n */
static void ensure_cache(idx_t n)
{
    /* already have enough */
    if (cache_size > n)
        return;

    /* we always need at least two elements to store the seeds */
    idx_t new_size = n + 1;
    if (new_size < 2)
        new_size = 2;

    /* reallocate the cache */
    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!p) {
        /* in a real program we would handle the error; here we abort */
        abort();
    }
    cache = p;

    /* initialise the seeds on a first run */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)4;   /* a(1) */
        cache_size = 2;
    }

    /* compute new values using the recurrence */
    for (idx_t i = cache_size; i < new_size; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term  = ( (seq_t)(i + 3) * prev ) - ( (seq_t)(i - 1) * prev2 );
        cache[i] = term;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
