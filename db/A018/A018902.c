/*
 * A018902  a(n+2) = 5*a(n+1) - 3*a(n).
 * https://oeis.org/A018902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 5*a(n+1) - 3*a(n), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms               */
static idx_t cache_len = 0;      /* number of stored terms                */

/* Compute the n-th term of OEIS sequence A018902. */
seq_t a(idx_t n)
{
    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure: return 0 as fallback */ }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failure: return 0 as fallback */ }
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)3 * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
