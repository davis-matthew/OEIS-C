/*
 * A170326  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 27 * Σ_{k=1}^{min(n,42)} a(n-k) where p(0) = 1,
 *            p(1) = p(2) = … = p(41) = 2, p(42) = 1, p(n) = 0 for n > 42.
 *            This follows from the given rational generating function ∑ a(n)
 *            t^n = (t^42 + 2 t^41 + … + 2 t + 1) / (378 t^42 – 27 t^41 – … –
 *            27 t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
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

static seq_t coeff[43] = {
    /* p(0) = 1                                     */
    (seq_t)1,
    /* p(1)…p(41) = 2                                 */
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    (seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,(seq_t)2,
    /* p(42) = 1                                      */
    (seq_t)1
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;   /* number of terms currently stored in cache */

/* Return a(n) for the Coxeter reduced word count sequence.
   The function dynamically grows the cache as needed and
   computes new terms only up to the requested index. */
seq_t a(idx_t n)
{
    /* Ensure capacity for index n (at least n+1 elements). */
    if ((idx_t)cache_cap <= n) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* Compute additional terms if necessary. */
    if ((idx_t)computed <= n) {
        for (idx_t i = computed; i <= n; ++i) {
            seq_t sum = 0;
            idx_t limit = (i < 42) ? i : 42;
            for (idx_t k = 1; k <= limit; ++k) {
                sum += cache[i - k];
            }
            seq_t p = (i <= 42) ? coeff[i] : (seq_t)0;
            cache[i] = p + (seq_t)27 * sum;
        }
        computed = n + 1;
    }

    return cache[n];
}
