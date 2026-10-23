/*
 * A174792  Expansion of x*(1 - x^2)/(1 - x + 7*x^2 + x^3).
 * https://oeis.org/A174792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174792: a(n) = a(n‑1) – 7*a(n‑2) – a(n‑3)
 * Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=−7. */
#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache to accommodate n if necessary */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failed */
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)0;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)1;
            else if (i == 3)     cache[i] = (seq_t)(-7);
            else                 cache[i] = cache[i-1] - (seq_t)7*cache[i-2] - cache[i-3];
        }
        cache_size = n + 1;
    }
    return cache[n];
}
