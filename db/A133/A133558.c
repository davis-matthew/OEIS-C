/*
 * A133558  a(n) = a(n-1) + 9*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialisation of the cache */
    if (cache == NULL) {
        cache = seq_alloc(2);
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
        cache_cap = 2;
    }

    /* If requested value is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + (cache[i-2] * (seq_t)9);
    }
    cache_len = n + 1;

    return cache[n];
}
