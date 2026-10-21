/*
 * A083239  First order recursion: a(0) = 1; a(n) = phi(n) - a(n-1) =
 *          A000010(n) - a(n-1).
 * https://oeis.org/A083239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = phi(n) - a(n-1) for n ≥ 1
 * Range:     i8 n <= 34, i16 n <= 568, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* Static cache for computed values and a flag array */
    static seq_t *cache = NULL;
    static bool *computed = NULL;
    static idx_t cache_size = 0;

    /* Grow the arrays if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        bool *new_comp = (bool *)realloc(computed, new_size * sizeof(bool));
        if (!new_comp) abort();
        computed = new_comp;

        /* Mark new entries as not computed */
        memset(&computed[cache_size], 0, (new_size - cache_size) * sizeof(bool));
        cache_size = new_size;
    }

    /* Base case */
    if (!computed[0]) {
        cache[0] = 1;
        computed[0] = true;
    }

    /* Compute values up to n lazily */
    for (idx_t i = 1; i <= n; ++i) {
        if (!computed[i]) {
            /* phi returns a seq_t; subtracting from the previous term */
            cache[i] = phi(i) - cache[i - 1];
            computed[i] = true;
        }
    }

    return cache[n];
}
