/*
 * A062178  a(n+1) = 2a(n)-a([n/2]) starting with a(0)=0 and a(1)=1.
 * https://oeis.org/A062178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(1)=1; a(n+1)=2*a(n)-a(floor(n/2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms on demand, caching previous values.
   Complexity: O(n) time for first request of a(n),
   O(1) for subsequent requests.  */

#include <stdlib.h>

/* internal cache for already computed terms             */
static seq_t *cache   = NULL;   /* cache[0..last_computed] */
static size_t  max_n  = 0;      /* number of stored terms (cache size) */

/* externally linked sequence function */
seq_t a(idx_t n)
{
    /* cast to size_t for array indexing (idx_t is uint64_t) */
    size_t idx = (size_t)n;
    /* if already computed, return directly */
    if (idx < max_n)
        return cache[idx];

    /* resize cache to hold up to idx */
    if (cache == NULL) {
        cache = (seq_t*)malloc((idx + 1) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* out of memory – graceful failure */
        /* initialize first two terms */
        cache[0] = (seq_t)0;
        if (idx >= 1)
            cache[1] = (seq_t)1;
        max_n = 2;
    } else if ((idx + 1) > max_n) {
        seq_t *tmp = (seq_t*)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* out of memory – graceful failure */
        cache = tmp;
        /* if expanding beyond previous max, initialise new portion
           (for indices that just became reachable but were not computed) */
        if (max_n < 2) {
            cache[0] = (seq_t)0;
            if (idx >= 1)
                cache[1] = (seq_t)1;
            max_n = 2;
        }
    }

    /* compute any missing terms up to idx */
    for (size_t i = max_n; i <= idx; ++i) {
        /* a(i) = 2*a(i-1) - a(floor((i-1)/2)) */
        size_t half = (i - 1) / 2;
        cache[i] = (seq_t)2 * cache[i - 1] - cache[half];
    }
    max_n = idx + 1;
    return cache[idx];
}
