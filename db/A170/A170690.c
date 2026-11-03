/*
 * A170690  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 7 * Σ_{k=1}^{49} a(n-k) - 28 * a(n-50) + p_n for
 *            n ≥ 0 where p_0 = 1, p_1 … p_49 = 2, p_50 = 1, and p_n = 0 for n
 *            > 50. This recurrence comes from the rational generating
 *            function (t^50 + 2t^49 + … + 2t + 1) / (28t^50 - 7t^49 - … - 7t
 *            + 1). The sequence is thus computed by a linea...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 187 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cache to keep already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;   /* number of terms currently stored (≥ 1) */

    /* make sure we have enough space for index n */
    if (n + 1 > cache_cap)
        ensure_cache(n + 1, &cache, &cache_cap);

    /* compute terms that are still missing */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;

        /* sum of the last 49 terms */
        for (int k = 1; k <= 49; ++k)
            if (i >= (idx_t)k)
                sum += cache[i - k];

        seq_t term = sum * (seq_t)7;

        if (i >= 50)
            term -= (seq_t)28 * cache[i - 50];

        /* add p_i, the numerator coefficient */
        if (i == 0)
            term += (seq_t)1;
        else if (i <= 49)
            term += (seq_t)2;
        else if (i == 50)
            term += (seq_t)1;
        /* for i > 50, p_i is 0, nothing to add */

        cache[i] = term;
    }

    computed = n + 1;
    return cache[n];
}
