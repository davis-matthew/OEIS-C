/*
 * A042711  Denominators of continued fraction convergents to sqrt(885).
 * https://oeis.org/A042711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 238*a(n-4) - a(n-8) for n > 7 a(0) = 1, a(1) = 1, a(2) =
 *            3, a(3) = 4, a(4) = 235, a(5) = 239, a(6) = 713, a(7) = 952
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for indices 0 .. 7 */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)4,
        (seq_t)235,(seq_t)239,(seq_t)713,(seq_t)952
    };

    /* Memoization: cache of previously computed terms */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cache_len = 0;   /* number of elements allocated */

    if (n < 8) {
        return init[n];
    }

    /* Allocate and initialise the cache on first use or when larger n is requested */
    if (cache_len == 0) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) {             /* allocation failure – return 0 */
            return (seq_t)0;
        }
        /* Store the first 8 terms */
        for (idx_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        /* Compute terms from 8 up to n */
        for (idx_t i = 8; i <= n; ++i) {
            cache[i] = (seq_t)238 * cache[i - 4] - cache[i - 8];
        }
        return cache[n];
    }

    /* If requested index is larger than the current cache, extend it */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {               /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute only the newly needed terms */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)238 * cache[i - 4] - cache[i - 8];
        }
        cache_len = new_len;
    }

    return cache[n];
}
