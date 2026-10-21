/*
 * A095004  a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3); given a(1) = 1, a(2) = 10,
 *          a(3) = 81.
 * https://oeis.org/A095004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3) with initial values a(1) =
 *            1, a(2) = 10, a(3) = 81
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values (1‑based indices stored at cache[i-1]) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of elements currently cached */

    /* Return immediately if the value is already cached */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Resize cache to accommodate up to index n */
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* If allocation fails, do not change existing cache; return 0 */
        return (seq_t)0;
    }

    /* Copy old values into new cache */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_cache[i] = cache[i];
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1u) {
            new_cache[0] = (seq_t)1;
        } else if (i == 2u) {
            new_cache[1] = (seq_t)10;
        } else if (i == 3u) {
            new_cache[2] = (seq_t)81;
        } else {
            /* recurrence: a(i) = 9*a(i-1) - 9*a(i-2) + a(i-3) */
            seq_t t1 = new_cache[i - 2];          /* a(i-1) */
            seq_t t2 = new_cache[i - 3];          /* a(i-2) */
            seq_t t3 = new_cache[i - 4];          /* a(i-3) */
            new_cache[i - 1] = (seq_t)(9) * t1 - (seq_t)(9) * t2 + t3;
        }
    }

    /* Free old cache and update pointers */
    free(cache);
    cache = new_cache;
    cache_size = new_size;

    return cache[n - 1];
}
