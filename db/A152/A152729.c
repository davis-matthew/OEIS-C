/*
 * A152729  a(n) = (n-2)^4 - a(n-1) - a(n-2), with a(1) = a(2) = 0.
 * https://oeis.org/A152729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)^4 - a(n-1) - a(n-2), a(1)=0, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 337, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence terms.
   cache[0] = a(1), cache[1] = a(2), … */
static seq_t *cache       = NULL;
static size_t cache_size  = 0;   /* how many terms are stored */
static size_t cache_cap   = 0;   /* allocated capacity */

/* Ensure that the cache can hold at least 'needed' terms.  */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;

    /* Use an explicit cast so the code is valid in C++ as well. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return 0;

    /* Initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0; /* a(1) */
        cache[1] = 0; /* a(2) */
        cache_size = 2;
    }

    if (n - 1 < cache_size)   /* value already computed */
        return cache[n - 1];

    /* Compute missing terms up to the requested index. */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* (k-2)^4  */
        seq_t tmp = (seq_t)(k - 2);
        seq_t term = tmp * tmp * tmp * tmp;

        /* Subtract a(k-1) and a(k-2) from the cache. */
        term -= cache[k - 2];   /* a(k-1) */
        term -= cache[k - 3];   /* a(k-2) */

        ensure_cache_capacity(cache_size + 1);
        cache[cache_size] = term;
        ++cache_size;
    }

    return cache[n - 1];
}
