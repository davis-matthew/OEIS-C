/*
 * A039569  Numbers whose base-5 representation has the same number of 1's,
 *          2's and 3's.
 * https://oeis.org/A039569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   brute force search for numbers whose base‑5 representation
 *            contains an equal number of 1, 2 and 3
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   equal_123
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_123.h"

/* Return the n-th term (1‑based) of A039569. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    static seq_t last_checked = 0;

    /* Return cached value if already computed. */
    if ((size_t)(n - 1) < cache_size) {
        return cache[n - 1];
    }

    /* Generate next terms until we have at least n. */
    while (cache_size < (size_t)n) {
        if (equal_123(last_checked)) {
            /* Grow cache if necessary. */
            if (cache_size >= cache_capacity) {
                size_t new_cap = (cache_capacity == 0) ? 16 : cache_capacity * 2;
                seq_t *new_cache;
#ifdef __cplusplus
                new_cache = new seq_t[new_cap];
                if (cache) {
                    for (size_t i = 0; i < cache_size; ++i) {
                        new_cache[i] = cache[i];
                    }
                    delete [] cache;
                }
#else
                new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
                cache = new_cache;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
