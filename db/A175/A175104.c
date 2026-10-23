/*
 * A175104  a(1)=1. For n >= 2, a(n) = 2*a(n-1) + (number of composites among
 *          first n-1 terms of the sequence).
 * https://oeis.org/A175104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 2*a(n-1) + (number of composites among the
 *            first n-1 terms of the sequence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t n_max = 0;
static idx_t composites = 0;

seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for indices below the offset */

    if (n > n_max) {
        /* Allocate or grow the cache to accommodate the new terms. */
        if (cache == NULL) {
            cache = (seq_t*)malloc(n * sizeof(seq_t));
            cache_cap = n;
        } else if (n > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed */
            cache = tmp;
            cache_cap = n;
        }

        /* Generate terms from (n_max + 1) up to n. */
        for (idx_t i = n_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];
                val = 2 * prev + composites;
            }
            cache[i - 1] = val;

            /* Update composite count after adding the new term. */
            if (val != (seq_t)1 && !is_prime_big(val))
                composites++;
        }

        n_max = n;
    }

    return cache[n - 1];
}
