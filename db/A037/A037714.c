/*
 * A037714  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2,0.
 * https://oeis.org/A037714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A037714: Base 9 digits are, in order, the first n terms of the
 * periodic sequence with initial period 1,3,2,0.
 * 
 * Recurrence:
 *     a(1) = 1
 *     a(n) = 9 * a(n-1) + d(n-1),    n ≥ 2
 * where d(k) = {1, 3, 2, 0}[k mod 4]  (periodic pattern).
 */

#include <stdlib.h>  /* malloc, realloc, free */
#include <string.h>  /* memcpy */

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)0;            /* offset is 1; n==0 is undefined */

    /* 0‑based index for internal array */
    size_t idx = (size_t)(n - 1u);

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0u;

    /* expand cache if needed and compute missing terms */
    if (idx >= cache_size) {
        /* allocate new array large enough for all requested terms */
        size_t new_size = idx + 1u;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure: return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* copy already computed terms */
        if (cache_size > 0u) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }

        /* constants for the digit pattern */
        static const unsigned char pattern[4] = {1, 3, 2, 0};

        /* initialize the first term if this is the first call */
        if (cache_size == 0u) {
            new_cache[0] = (seq_t)1;            /* a(1) = 1 */
            cache_size = 1u;
        }

        /* compute remaining terms up to the requested index */
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t prev = new_cache[i - 1];
            new_cache[i] = prev * (seq_t)9 + (seq_t)pattern[i % 4];
        }

        /* update cache pointers */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[idx];
}
