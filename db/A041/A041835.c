/*
 * A041835  Denominators of continued fraction convergents to sqrt(438).
 * https://oeis.org/A041835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   * a(n) = 586*a(n-4) - a(n-8) for n > 7 * a(0) = 1, a(1) = 1,
 *            a(2) = 13, a(3) = 14, * a(4) = 573, a(5) = 587, a(6) = 7617,
 *            a(7) = 8204
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated capacity   */
static size_t cache_size = 0;   /* number of values stored */

/* Ensure cache is initialized with the first 8 terms */
static void cache_init(void)
{
    if (cache != NULL)
        return;
    /* Allocate enough space for the first 8 terms */
    cache_cap = 8;
    cache = mem_new(seq_t, cache_cap);
    if (!cache)
        return;  /* out of memory – unlikely for n <= 10000 */

    /* First 8 terms */
    cache[0] = 1;
    cache[1] = 1;
    cache[2] = 13;
    cache[3] = 14;
    cache[4] = 573;
    cache[5] = 587;
    cache[6] = 7617;
    cache[7] = 8204;

    cache_size = 8;
}

/* Resize the cache if needed.  Assume 'needed' <= ULONG_MAX. */
static void cache_expand(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr)
        return;  /* out of memory – unlikely */

    mem_copy(new_arr, cache, cache_size * sizeof(seq_t));
    mem_free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Public function */
seq_t a(idx_t n)
{
    /* Ensure the cache has been created and the first 8 terms are in place */
    cache_init();

    /* If the requested term is beyond what we have computed, extend the cache */
    if ((size_t)n >= cache_size) {
        cache_expand((size_t)n + 1);
        /* Compute new terms using the recurrence relation */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 586*a(i-4) - a(i-8) */
            cache[i] = cache[i - 4] * (seq_t)586 - cache[i - 8];
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}
