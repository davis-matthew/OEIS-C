/*
 * A016153  a(n) = (9^n-4^n)/5.
 * https://oeis.org/A016153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_cache, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A016153: a(n) = (9^n - 4^n)/5,  a(0)=0, a(1)=1
 * Recurrence for efficiency: a(n) = 13*a(n-1) - 36*a(n-2)
 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/seq_alloc.h"

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of values currently stored */
static size_t cache_cap = 0; /* current allocation capacity */

seq_t a(idx_t n)
{
    /* Handle trivial cases optimally */
    if (n == 0) return (seq_t)0;
    /* Initialize cache for first two terms if not done yet */
    if (cache_len == 0) {
        ensure_cache(2, &cache, &cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }
    /* Grow and fill cache up to requested n */
    if ((size_t)n >= cache_len) {
        ensure_cache((size_t)n + 1, &cache, &cache_cap);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)13 - cache[i - 2] * (seq_t)36;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
