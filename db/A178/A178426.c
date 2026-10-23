/*
 * A178426  11 followed by the Fermat numbers A152585.
 * https://oeis.org/A178426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(0) = 11; a(n) = 2 + product_{i=0}^{n-1} a(i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static seq_t prod   = 1;   /* product of all a[0..size-1] */

    size_t idx = (size_t)n;

    /* initialize for n == 0 if needed */
    if (size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure (unlikely) */
        cap   = 1;
        size  = 1;
        cache[0] = (seq_t)11;
        prod    = cache[0];
    }

    if (idx < size)
        return cache[idx];

    /* grow the cache to reach index idx */
    if (idx + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    for (size_t i = size; i <= idx; ++i) {
        seq_t val = (seq_t)2 + prod;   /* a(i) = 2 + product of previous terms */
        cache[i] = val;
        prod *= val;                    /* update product for next iteration */
    }
    size = idx + 1;
    return cache[idx];
}
