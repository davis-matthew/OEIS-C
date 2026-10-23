/*
 * A027983  Duplicate of A027974.
 * https://oeis.org/A027983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) satisfies a(0)=1, a(1)=5, a(2)=14, and for n≥3: a(n) =
 *            3·a(n‑1) – a(n‑2) – 2·a(n‑3) (Linear recurrence)
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of computed terms */
    static idx_t cache_size = 0;    /* number of stored terms */

    /* Expand cache if the requested index is not yet computed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);      /* allocation failed */

        /* copy old values if any */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;

        /* compute terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)5;
            else if (i == 2)
                cache[i] = (seq_t)14;
            else
                cache[i] = (seq_t)3 * cache[i-1] - cache[i-2] - (seq_t)2 * cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
