/*
 * A052606  E.g.f. (1-x)^2/(1-4x+x^2).
 * https://oeis.org/A052606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   safe_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052606
   E.g.f.: (1-x)^2/(1-4x+x^2)
   Recurrence (n ≥ 1):
     a(n+2) = (4n+8)*a(n+1) – (n^2+3n+2)*a(n)
   Initial values: a(0)=1, a(1)=2, a(2)=16
   (the recurrence does not hold for n=0; a(2) is defined separately)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/safe_mul.h"

/* Cached values */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cached_max = (idx_t)-1; /* highest index computed so far */
static size_t cache_cap  = 0;    /* current allocated capacity */

/* Ensure that terms up to index n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    /* If we have already computed up to n, nothing to do. */
    if (cached_max != (idx_t)-1 && n <= cached_max)
        return;

    /* Grow the cache if necessary. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4)
        new_cap = 4;
    if (!cache) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else if (new_cap > cache_cap) {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;

    /* Determine where to start computing. */
    idx_t start;
    if (cached_max == (idx_t)-1)
        start = 0;
    else
        start = cached_max + 1;

    /* Initialise the first few terms if needed. */
    if (start == 0) {
        cache[0] = (seq_t)1;
        start = 1;
    }
    if (start == 1) {
        cache[1] = (seq_t)2;
        start = 2;
    }
    if (start == 2) {
        cache[2] = (seq_t)16;
        start = 3;
    }

    /* Compute remaining terms up to n using the recurrence. */
    for (idx_t i = start; i <= n; ++i) {
        /* Recurrence requires n >= 1, i.e., i >= 3. */
        seq_t term1 = safe_mul(cache[i - 2], (uint64_t)((i - 2) * (i - 2) + 3 * (i - 2) + 2));
        seq_t term2 = safe_mul(cache[i - 1], (uint64_t)(4 * (i - 2) + 8));
        cache[i] = term2 - term1;
    }

    cached_max = n;
}

/* Public sequence accessor. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
