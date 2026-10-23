/*
 * A010904  Pisot sequence E(4,14): a(n) = floor(a(n-1)^2/a(n-2)+1/2) for n>1,
 *          a(0)=4, a(1)=14.
 * https://oeis.org/A010904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=4, a(1)=14, a(2)=49, a(n)=4*a(n-1)-2*a(n-2)+a(n-3) (n>2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cache_cap = 0;    /* capacity of cache array */
static idx_t cache_size = 0;    /* number of terms stored (0 … cache_size-1) */

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Make sure we have enough space for terms 0 … n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms if they aren't stored yet */
    if (cache_size < 3) {
        if (cache_cap < 3) {
            seq_t *tmp = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = 3;
        }
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)49;
        cache_size = 3;
    }

    /* Compute terms up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)4;
        term -= cache[i - 2] * (seq_t)2;
        term += cache[i - 3];
        cache[i] = term;
    }

    cache_size = n + 1;    /* update the number of cached terms */
    return cache[n];
}
