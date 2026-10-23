/*
 * A045161  Numbers whose base-5 representation contains exactly one 0 and no
 *          1's.
 * https://oeis.org/A045161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4774, u32 n <= 7798, u64 n <= 7798,
 *            u128 n <= 7798, big any n
 * Helpers:   valid_base5_onezero_noone
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-5 representation contains exactly one 0 and no 1's. */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/valid_base5_onezero_noone.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cached values */
    static seq_t *cache = NULL;      /* array of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static size_t cache_len = 0;     /* number of terms stored */
    static idx_t next_start = 0;     /* next integer to examine */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        assert(cache);
        cache_len = 0;
        next_start = 0;
    }

    /* Populate cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t candidate = (seq_t)next_start;
        next_start++;

        if (valid_base5_onezero_noone(candidate)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
                assert(new_cache);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];                /* a(1) is stored at index 0 */
}
