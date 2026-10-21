/*
 * A074863  a(n) = a(n-1) + a(n-2) + a(n-3) + R(a(n-4)) where
 *          a(0)=a(1)=a(2)=a(3)=1 and R(n) (A004086) is the reverse of n.
 * https://oeis.org/A074863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + R(a(n-4))
 *            a(0)=a(1)=a(2)=a(3)=1, where R(x) is the decimal reverse of x
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   reverse_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_value.h"

/* Cached array of already computed terms. */
static seq_t *cache = NULL;          /* pointer to the cache array       */
static idx_t cache_alloc = 0;        /* number of elements allocated     */
static idx_t cache_computed = 0;     /* number of elements already computed */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache if needed. */
    if (cache == NULL)
    {
        /* Start with a modest size; will grow as necessary. */
        cache_alloc = 16;
        cache = (seq_t *)malloc((size_t)cache_alloc * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
    }
    else if (n + 1 > cache_alloc)
    {
        /* Grow to at least n+1 elements. */
        idx_t new_alloc = cache_alloc;
        while (n + 1 > new_alloc) new_alloc <<= 1;   /* double until fit */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_alloc * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* If the term is already known, return it. */
    if (n < cache_computed)
        return cache[n];

    /* Compute any missing terms up to n (inclusive). */
    for (idx_t i = cache_computed; i <= n; ++i)
    {
        if (i < 4)
            cache[i] = 1;
        else
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                     + reverse_value(cache[i - 4]);
    }

    cache_computed = n + 1;   /* all indices < n+1 are now computed */
    return cache[n];
}
