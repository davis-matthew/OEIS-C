/*
 * A143985  a(0) = 0; thereafter, a(n) = (a(n - 1) + 2)^2 - n.
 * https://oeis.org/A143985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = (a(n-1) + 2)^2 - n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache to store computed values.
       Cache[0] is initialized on first use. */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache */
    static idx_t size = 0;          /* number of values stored (>=1 if cache[0] set) */

    /* If the desired index is already stored, return it. */
    if (n < size) {
        return cache[n];
    }

    /* Determine new capacity needed (double until > n). */
    size_t new_cap = cap ? cap : 1;
    while (new_cap <= n) {
        new_cap <<= 1;
    }

    /* Reallocate the cache. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – return 0 (undefined for large n) */
        return (seq_t)0;
    }
    cache = tmp;
    cap = new_cap;

    /* If this is the first call, set a(0). */
    if (size == 0) {
        cache[0] = (seq_t)0;
        size = 1;
    }

    /* Compute values from current size up to n. */
    for (idx_t i = size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t tmp2 = prev + (seq_t)2;
        seq_t val   = tmp2 * tmp2;
        val -= (seq_t)i;            /* a(i) = (a(i-1)+2)^2 - i */
        cache[i] = val;
    }
    size = n + 1;   /* now cache[0..n] are filled */

    return cache[n];
}
