/*
 * A098291  Chebyshev polynomials S(n,731) + S(n-1,731) with Diophantine
 *          property.
 * https://oeis.org/A098291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = U(n,731/2)+U(n-1,731/2) = 731*a(n-1) - a(n-2); initial
 *            values: a(0)=1, a(1)=732
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void grow_cache(idx_t n)
{
    if (cache_cap > n)
        return;
    /* reallocate to hold indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        abort(); /* memory allocation failed */
    cache = new_cache;
    /* compute new entries */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)732;
        else
            cache[i] = (seq_t)731 * cache[i - 1] - cache[i - 2];
    }
    cache_cap = n + 1;
}

seq_t a(idx_t n)
{
    if (n >= cache_cap)
        grow_cache(n);
    return cache[n];
}
