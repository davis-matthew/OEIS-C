/*
 * A041797  Denominators of continued fraction convergents to sqrt(419).
 * https://oeis.org/A041797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   compute_initial_36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A041797: Denominators of continued fraction convergents to sqrt(419).
   Recurrence: a(n) = 540349940*a(n-18) - a(n-36) for n>35
   Initial terms for 0 ≤ n < 36 are obtained from the continued‑fraction
   expansion of sqrt(419). The first 36 values are computed directly.
   The function supports arbitrary order of calls by memoizing the
   computed values.
*/

#include <stdlib.h>
#include "../utils/compute_initial_36.h"

/* Global cache for memoization. */
static seq_t *cache   = NULL;   /* array holding a(n) values */
static size_t cache_len = 0;    /* number of cached terms (max index + 1) */

seq_t a(idx_t n)
{
    /* Allocate or enlarge the cache so that it can hold a(n). */
    if (!cache) {
        /* At least 36 entries are needed for the recurrence initialization. */
        size_t alloc = n + 1;
        if (alloc < 36) alloc = 36;
        cache = (seq_t *)malloc(sizeof(seq_t) * alloc);
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache_len = 0;
    } else if (cache_len <= n) {
        size_t alloc = n + 1;
        if (alloc < 36) alloc = 36;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * alloc);
        if (!tmp) return (seq_t)0;    /* allocation failed */
        cache = tmp;
    }

    /* Compute the first 36 terms if they are not yet cached. */
    if (cache_len == 0) {
        compute_initial_36(cache);
        cache_len = 36;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len) return cache[n];

    /* Use the recurrence to fill in the cache up to index n. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val = (seq_t)540349940 * cache[k - 18] - cache[k - 36];
        cache[k] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
