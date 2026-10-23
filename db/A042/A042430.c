/*
 * A042430  Numerators of continued fraction convergents to sqrt(743).
 * https://oeis.org/A042430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A042430: Numerators of continued‑fraction convergents to sqrt(743).   
   Partial quotients (excluding a0) repeat with period [3,1,7,27,7,1,3,54].  
   Let a0 = 27.  
   Recurrence: a(0) = 27, a(1) = 3*a0 + 1 = 82,  
              a(n) = period[(n-1) % 8] * a(n-1) + a(n-2)  for n ≥ 2
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values a(0)...a(cache_size-1) */
static idx_t  cache_size = 0;        /* number of cached entries */

/* Periodic part of the continued fraction of sqrt(743) */
static const idx_t period[8] = {3, 1, 7, 27, 7, 1, 3, 54};
static const idx_t period_len = 8;
static const idx_t a0 = 27;

seq_t a(idx_t n)
{
    /* Ensure we have at least two values cached. */
    if (cache_size < 2) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – unlikely */

        cache[0] = (seq_t)a0;              /* a(0) */
        cache[1] = (seq_t)period[0] * cache[0] + (seq_t)1; /* a(1) = 3*a0 + 1 */
        cache_size = 2;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to include the required term. */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) return 0;                   /* allocation failure */
    cache_size = n + 1;

    /* Compute missing values up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t coef_index = (i - 1) % period_len;
        cache[i] = (seq_t)period[coef_index] * cache[i - 1] + cache[i - 2];
    }

    return cache[n];
}
