/*
 * A013025  Expansion of e.g.f. exp(sinh(x) + sin(x)).
 * https://oeis.org/A013025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Σ_{k=0..⌊(n‑1)/4⌋} binomial(n‑1,4k) *
 *            a(n-4k-1) (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */

/* Optional: include <stdbool.h> if needed elsewhere but not used here. */

/* --- Memoization cache --------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_used  = 0;      /* number of entries that have been set */
static size_t cache_cap   = 0;      /* allocated capacity of the array */

/* Ensure the cache has room for at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* Allocation failure */

    cache     = tmp;
    cache_cap = new_cap;
}

/* --- The sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialize cache with the first value a(0) = 1 */
    if (cache_used == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache_used = 1;
    }

    /* If the desired index has not yet been computed, grow the cache */
    if ((size_t)n >= cache_used) {
        size_t needed = (size_t)n + 1;
        ensure_capacity(needed);

        for (idx_t i = (idx_t)cache_used; i <= n; ++i) {
            /* i >= 1 by construction */
            idx_t upper = (i - 1) / 4;
            seq_t sum = (seq_t)0;

            for (idx_t k = 0; k <= upper; ++k) {
                idx_t prev_idx = i - 4 * k - 1;     /* a(prev_idx) is already known */
                seq_t comb     = binomial(i - 1, 4 * k);
                sum += comb * cache[prev_idx];
            }
            cache[i] = (seq_t)(2 * sum);
        }

        cache_used = (size_t)n + 1;
    }

    return cache[n];
}
