/*
 * A065705  a(n) = Lucas(10*n).
 * https://oeis.org/A065705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 123*a(n-1) - a(n-2), a(0)=2, a(1)=123
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* make sure cache is large enough and contains all values up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed – terminate */
    cache = tmp;

    /* initialise the new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = 2;          /* Lucas(0) = 2 */
        else if (i == 1)
            cache[i] = 123;        /* Lucas(10) = 123 */
        else
            cache[i] = 123 * cache[i - 1] - cache[i - 2];
    }

    cache_size = (idx_t)new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
