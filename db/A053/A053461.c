/*
 * A053461  a(0) = 0; a(n) = a(n-1) - n^2 if positive and new, otherwise a(n)
 *          = a(n-1) + n^2.
 * https://oeis.org/A053461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(n) = a(n-1) - n² if the result is positive and distinct from all previous terms,
          otherwise a(n) = a(n-1) + n² */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains.h"

/* Static cache for already computed terms (grows on demand). */
static seq_t *cache = NULL;     /* array of a(0) .. a(k-1) when cache_size == k   */
static idx_t  cache_size = 0;   /* number of terms currently stored            */
static idx_t  cache_cap   = 0;   /* allocated array capacity                    */

/* Ensure the cache can hold at least `need` terms. */
static inline void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* External linkage required by the problem statement. */
seq_t a(idx_t n)
{
    /* If we already have the value, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate term n. */
    ensure_cache_capacity(n + 1);

    /* Initialize the first term if this is the first request. */
    if (cache_size == 0) {
        cache[0] = 0;
        cache_size = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sq   = (seq_t)i * (seq_t)i;   /* i², cast to seq_t first */
        seq_t cand = prev - sq;

        if (prev > sq) {
            /* Candidate is positive, check if it is new. */
            if (cand > 0 && !contains(cache, i, cand))
                cache[i] = cand;
            else
                cache[i] = prev + sq;
        } else {
            /* Adding would guarantee positivity; use + i². */
            cache[i] = prev + sq;
        }
    }

    if (n + 1 > cache_size)
        cache_size = n + 1;

    return cache[n];
}
