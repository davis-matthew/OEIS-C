/*
 * A050542  Iterated triangular numbers with seed 5.
 * https://oeis.org/A050542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = C(a(n-1)+1, 2) with a(0) = 5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed values.  It grows
       lazily as needed, so calls in any order are handled efficiently. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache for the first call. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* unlikely in tests */
        cache[0] = (seq_t)5;
        cache_size = 1;
    }

    /* Return cached value if it has already been computed. */
    if (n < cache_size)
        return cache[n];

    /* Grow cache and compute missing values iteratively. */
    /* We avoid recursion to keep the stack depth constant. */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0; /* allocation failure */
    cache = new_cache;
    cache_size = new_size;

    for (idx_t k = old_size; k <= n; ++k) {
        seq_t prev = cache[k - 1];
        /* a(k) = C(prev + 1, 2) = (prev + 1) * prev / 2 */
        cache[k] = (prev + (seq_t)1) * prev / (seq_t)2;
    }

    return cache[n];
}
