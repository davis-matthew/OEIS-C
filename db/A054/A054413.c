/*
 * A054413  a(n) = 7*a(n-1) + a(n-2), with a(0)=1 and a(1)=7.
 * https://oeis.org/A054413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Recurrence: a(n) = 7*a(n-1) + a(n-2), a(0) = 1, a(1) = 7 */

seq_t a(idx_t n) {
    /* Static cache to hold computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* Initialise cache with the first two terms */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache_size = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    }

    /* Return already computed value if possible */
    if (n < cache_size) {
        return cache[n];
    }

    /* Expand cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n + 1) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failure – unlikely in tests */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms up to index n and store them */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t next = ((seq_t)7) * cache[i - 1] + cache[i - 2];
        cache[i] = next;
    }

    cache_size = n + 1;
    return cache[n];
}
