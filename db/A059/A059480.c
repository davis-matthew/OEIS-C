/*
 * A059480  a(0) = a(1) = 1; a(n) = a(n-1) + (n+1)*a(n-2).
 * https://oeis.org/A059480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1; a(n)=a(n-1)+(n+1)*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t  cache_size = 0;       /* number of valid entries in cache */
static size_t cache_cap   = 0;       /* allocated size of cache */

/* external function as required */
seq_t a(idx_t n)
{
    /* Initialise cache if this is the first call */
    if (cache == NULL) {
        cache_cap  = 64;                         /* initial capacity */
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0]   = (seq_t)1;                  /* a(0) = 1 */
        cache[1]   = (seq_t)1;                  /* a(1) = 1 */
        cache_size = 2;                         /* two entries stored */
    }

    /* Fill the cache up to the requested index */
    while (cache_size <= n) {
        idx_t k = cache_size;                   /* compute a(k) */

        /* a(k) = a(k-1) + (k+1)*a(k-2) */
        seq_t term = cache[k-1] + ((seq_t)(k + 1)) * cache[k-2];

        /* Grow the cache if necessary */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_size++] = term;
    }

    return cache[n];
}
