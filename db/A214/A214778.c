/*
 * A214778  a(n) = 3*a(n-1) + 6*a(n-2) + a(n-3), with a(0) = 3, a(1) = 3, and
 *          a(2) = 21.
 * https://oeis.org/A214778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) + a(n-3), a(0) = 3, a(1) = 3, a(2) =
 *            21
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size   = 0;
static idx_t  max_cached   = (idx_t)-1; /* sentinel meaning none cached yet */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)21;

    /* Ensure cache capacity */
    if (cache == NULL) {
        cache         = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        cache_size    = n + 1;
        max_cached    = 2;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)21;
    } else if ((size_t)n + 1 > cache_size) {
        cache         = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        cache_size    = n + 1;
    }

    /* If we already have the value, return it */
    if (n <= max_cached)
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] + 6 * cache[i - 2] + cache[i - 3];
    }
    max_cached = n;

    return cache[n];
}
