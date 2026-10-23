/*
 * A108136  a(1)=1; a(2)=1; a(3)=1; a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3).
 * https://oeis.org/A108136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3), a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of previously computed terms */
static idx_t  cache_cap = 0;     /* allocated cache capacity */
static idx_t  cache_size = 0;    /* number of terms stored in the cache */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Initialise cache storage if needed */
    if (cache == NULL) {
        cache_cap = 8;                   /* arbitrary initial size */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
    }

    /* Grow cache to accommodate the requested index */
    if (cache_size < n) {
        if (cache_cap < n) {
            idx_t newcap = cache_cap;
            while (newcap < n) newcap <<= 1;   /* double until large enough */
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;         /* allocation failure */
            cache = tmp;
            cache_cap = newcap;
        }

        /* Fill base cases if they are not yet stored */
        if (cache_size == 0) {
            cache[0] = (seq_t)1; cache_size = 1;
        }
        if (cache_size == 1) {
            cache[1] = (seq_t)1; cache_size = 2;
        }
        if (cache_size == 2) {
            cache[2] = (seq_t)1; cache_size = 3;
        }

        /* Compute missing terms sequentially */
        for (idx_t i = cache_size; i < n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            cache[i] = 3*a1 + 2*a2 + a3;
        }
        cache_size = n;
    }

    return cache[n-1];   /* a(1) is stored at index 0 */
}
