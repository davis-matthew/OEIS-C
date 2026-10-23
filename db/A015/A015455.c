/*
 * A015455  a(n) = 9*a(n-1) + a(n-2) for n>1; a(0) = a(1) = 1.
 * https://oeis.org/A015455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

/* Ensure cache can hold indices up to 'upto' (exclusive). */
static void ensure_cache(idx_t upto)
{
    if (upto <= cache_size)
        return;

    /* Grow capacity if necessary. */
    if (cache_cap < upto) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < upto)
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing entries. */
    for (idx_t i = cache_size; i < upto; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 1;
        else
            cache[i] = 9 * cache[i - 1] + cache[i - 2];
    }
    cache_size = upto;
}

seq_t a(idx_t n)
{
    ensure_cache(n + 1);
    return cache[n];
}
