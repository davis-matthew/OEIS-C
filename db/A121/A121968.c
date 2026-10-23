/*
 * A121968  a(n) = 2*a(n-1) - a(n-2) + n + 1.
 * https://oeis.org/A121968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 2952, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + n,  a(1)=1, a(2)=2
   (equivalently a(n) = C(n+1,3) + n^2 + 1) */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms, index 0 unused */
static idx_t  cache_len = 0;    /* number of allocated slots (cache[0..cache_len-1]) */
static idx_t  last_idx  = 0;    /* highest index already computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* values below the OEIS offset are irrelevant */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 3;                          /* indices 0,1,2 needed */
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) exit(1);             /* allocation failure */
        cache[0] = 0;                            /* unused */
        cache[1] = 1;                            /* a(1) */
        cache[2] = 2;                            /* a(2) */
        last_idx = 2;
    }

    /* grow the cache if necessary */
    if ((idx_t)n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        /* initialise any newly allocated slots to zero */
        for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing terms using the recurrence */
    if (n > last_idx) {
        for (idx_t i = last_idx + 1; i <= n; ++i) {
            cache[i] = ((seq_t)2 * cache[i-1]) - cache[i-2] + (seq_t)i;
        }
        last_idx = n;
    }

    return cache[n];
}
