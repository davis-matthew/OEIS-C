/*
 * A115113  a(n) = 3*a(n-1) + 4*a(n-2), with a(0) = 2, a(1) = 6, a(2) = 10.
 * https://oeis.org/A115113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 6, a(2) = 10, a(n) = 3*a(n-1) + 4*a(n-2) for n
 *            ≥ 3 Memoised for efficient repeated calls
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_next = 0;

/* enlarge the cache up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n < cache_next)
        return cache[n];

    for (idx_t i = cache_next; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)2;
        else if (i == 1)      cache[i] = (seq_t)6;
        else if (i == 2)      cache[i] = (seq_t)10;
        else                  cache[i] = (seq_t)3 * cache[i - 1]
                                     + (seq_t)4 * cache[i - 2];
    }

    cache_next = n + 1;
    return cache[n];
}
