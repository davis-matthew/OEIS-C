/*
 * A100545  Expansion of (7-2*x) / (1-3*x+x^2).
 * https://oeis.org/A100545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2); a(0)=7, a(1)=19
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/resize_array.h"

static seq_t *cache = NULL;   /* Cached sequence values */
static idx_t cache_size = 0;  /* Number of values stored in cache */

seq_t a(idx_t n)
{
    /* Return cached value if it exists */
    if (n < cache_size)
        return cache[n];

    /* Remember old size and grow cache to accommodate n */
    idx_t old_size = cache_size;
    seq_t *new_cache = resize_array(cache, old_size, n + 1);
    cache = new_cache;

    /* Compute missing values using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)7;
        } else if (i == 1) {
            cache[1] = (seq_t)19;
        } else {
            /* a(i) = 3*a(i-1) - a(i-2) */
            cache[i] = (seq_t)3 * cache[i - 1] - cache[i - 2];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
