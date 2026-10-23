/*
 * A101946  a(n) = 6*2^n - 3*n - 5.
 * https://oeis.org/A101946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - 3*n - 5 Equivalent recurrence: a(0) = 1 a(n) =
 *            2*a(n-1) + 3*n - 1 for n >= 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>

/* Static cache for previously computed values */
static seq_t *cache = NULL;     /* array of computed a(n) */
static idx_t cache_len = 0;    /* number of values stored (next free index) */
static idx_t cache_cap = 0;    /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized and large enough */
    if (cache == NULL) {
        cache_cap = 1;                    /* start with capacity 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow the cache if needed */
    while (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 1;    /* guard against 0 when cache_cap == 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – in this minimal implementation we abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values up to n */
    while (cache_len <= n) {
        idx_t i = cache_len;
        if (i == 0) {
            cache[0] = (seq_t)1;           /* a(0) = 1 */
        } else {
            /* a(i) = 2*a(i-1) + 3*i - 1 */
            cache[i] = cache[i - 1] * (seq_t)2
                     + (seq_t)((idx_t)3 * i)
                     - (seq_t)1;
        }
        cache_len = i + 1;
    }

    return cache[n];
}
