/*
 * A041337  Denominators of continued fraction convergents to sqrt(182).
 * https://oeis.org/A041337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 54*a(n-2) - a(n-4), a(0)=1, a(1)=2, a(2)=53, a(3)=108
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases for n < 4 */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 53;
    if (n == 3) return 108;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first four terms on first use */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 53;
        cache[3] = 108;
        cache_size = 4;
    }

    /* If requested value is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Remember the size before growing the cache */
    idx_t old_size = cache_size;

    /* Grow the cache if necessary */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute terms from old_size up to n using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = (seq_t)54 * cache[i - 2] - cache[i - 4];
    }

    return cache[n];
}
