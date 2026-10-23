/*
 * A162356  a(n) = 8*a(n-1)-14*a(n-2) for n>1; a(0) = 2; a(1) = 9.
 * https://oeis.org/A162356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), a(0) = 2, a(1) = 9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_until = 0; /* highest index computed */

    /* Make sure we have space for index n */
    if (n + 1 > cache_size) {
        seq_t *new_cache = seq_alloc_array(n + 1);
        if (cache != NULL) {
            /* copy already computed terms */
            for (idx_t i = 0; i <= computed_until; ++i) {
                new_cache[i] = cache[i];
            }
            free_seq_array(cache);
        }
        /* set the base case for a fresh array */
        new_cache[0] = (seq_t)2;
        cache = new_cache;
        cache_size = n + 1;
        /* initialize the second term if the array is large enough */
        if (n >= 1 && cache_size >= 2) {
            cache[1] = (seq_t)9;
            computed_until = 1;
        } else {
            computed_until = 0;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        cache[i] = 8 * cache[i-1] - 14 * cache[i-2];
    }
    computed_until = n;

    return cache[n];
}
