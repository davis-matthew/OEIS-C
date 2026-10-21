/*
 * A097768  Chebyshev U(n,x) polynomial evaluated at x=289=2*12^2+1.
 * https://oeis.org/A097768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*289*a(n-1) - a(n-2), a(0)=1, a(1)=578
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Initialise cache for the first two terms if not done yet */
    if (cache_len < 2) {
        size_t init_len = 2;
        if (cache == NULL) {
            cache = (seq_t*)malloc(sizeof(seq_t) * init_len);
            if (cache == NULL) return (seq_t)0;      /* out of memory */
        } else {
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * init_len);
            if (cache == NULL) return (seq_t)0;      /* out of memory */
        }
        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)578;     /* a(1) */
        cache_len = 2;
    }

    /* If the requested index is already computed, return it */
    if (n < cache_len) return cache[n];

    /* Expand the cache to include the n-th term */
    size_t new_len = (size_t) (n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL) return (seq_t)0;          /* out of memory */
    cache = tmp;

    /* Initialise new entries to zero (safe for uint* and big‑integer types) */
    for (idx_t i = cache_len; i < new_len; ++i)
        cache[i] = (seq_t)0;

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 578*a(i-1) - a(i-2) */
        cache[i] = (seq_t)578 * cache[i-1] - cache[i-2];
    }

    cache_len = n + 1;
    return cache[n];
}
