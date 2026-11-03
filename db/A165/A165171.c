/*
 * A165171  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(0)=1, a(1)=39, … ,
 *            a(7)=117426518976, a(8)=37·Σ_{k=1}^{7} a(8−k)−703·a(0)+1,
 *            a(n)=37·Σ_{k=1}^{7} a(n−k)−703·a(n−8) for n≥9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size of cache */
    static idx_t  computed = 0;           /* highest index computed so far */

    /* initialise with the first 8 terms when first called */
    if (cache == NULL) {
        /* ensure the cache can hold indices 0..8 */
        ensure_cache(8, &cache, &cache_cap);

        /* Initial known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)39;
        cache[2] = (seq_t)1482;
        cache[3] = (seq_t)56316;
        cache[4] = (seq_t)2140008;
        cache[5] = (seq_t)81320304;
        cache[6] = (seq_t)3090171552;
        cache[7] = (seq_t)117426518976;

        /* compute a(8) with the +1 offset coming from the generating function */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k)
            sum += cache[8 - k];
        sum *= (seq_t)37;
        cache[8] = sum - ((seq_t)703 * cache[0]) + (seq_t)1;

        computed = 8;
    }

    /* compute any further terms on demand */
    if (n > computed) {
        /* ensure the cache can hold up to index n */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t i = computed + 1; i <= n; ++i) {
            /* sum of the previous seven terms */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 7; ++k)
                sum += cache[i - k];

            seq_t term = sum * (seq_t)37 - (seq_t)703 * cache[i - 8];
            cache[i] = term;
        }
        computed = n;
    }

    return cache[n];
}
