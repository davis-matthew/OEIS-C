/*
 * A212364  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 5).
 * https://oeis.org/A212364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = a(n-1) + ∑_{k=1}^{n-5} a(k) *
 *            a(n-5-k) (empty sum = 0)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 48, u64 n <= 89, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;     /* array of a-values */
    static size_t cache_size = 0;   /* number of allocated slots */
    static idx_t max_computed = 0;  /* highest index already filled */

    /* Ensure the cache can hold up to index n */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = cache;

        if (cache == NULL) {
            /* Initial allocation */
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
            for (size_t i = 0; i < new_size; ++i)
                new_cache[i] = (seq_t)0;
            new_cache[0] = (seq_t)1;   /* a(0) = 1 */
        } else {
            /* Grow the array */
            size_t old_size = cache_size;
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
            for (size_t i = old_size; i < new_size; ++i)
                new_cache[i] = (seq_t)0;
            /* a(0) is already set */
        }

        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    if (n > max_computed) {
        seq_t *c = cache;
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            if (i >= 5) {
                for (idx_t k = 1; k <= i - 5; ++k) {
                    sum += c[k] * c[i - 5 - k];
                }
            }
            c[i] = c[i - 1] + sum;
        }
        max_computed = n;
    }

    return cache[n];
}
