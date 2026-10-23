/*
 * A159618  Numerator of Hermite(n, 2/19).
 * https://oeis.org/A159618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19^n * Hermite(n,2/19) Recurrence: a(0)=1, a(1)=4,
 *            a(n)=4*a(n-1)-722*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to reuse previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements currently stored */

    if (n >= cache_cap) {
        /* enlarge cache to hold terms up to n */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failed – undefined behaviour, but avoid crash */
            return 0;

        /* copy old terms */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* initialise and compute the missing terms */
        if (new_cap > 0)
            new_cache[0] = 1;            /* a(0) */
        if (new_cap > 1)
            new_cache[1] = 4;            /* a(1) */
        for (idx_t i = 2; i < new_cap; ++i) {
            /* a(i) = 4*a(i‑1) – 722*(i‑1)*a(i‑2) */
            new_cache[i] = 4 * new_cache[i - 1]
                           - (seq_t)722 * (seq_t)(i - 1) * new_cache[i - 2];
        }

        /* free old cache and replace it */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[n];
}
