/*
 * A231665  a(n) = Sum_{i=0..n} digsum_4(i)^2, where digsum_4(i) = A053737(i).
 * https://oeis.org/A231665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A231665
   Formula: a(n) = ∑_{i=0..n} digsum_4(i)^2
   where digsum_4(i) is the sum of the base‑4 digits of i.  */

#include <stdlib.h>   /* for malloc, free */
#include <stddef.h>   /* for size_t */

static seq_t *cache   = NULL;   /* table of already computed a(i) */
static size_t cache_sz = 0;     /* number of entries in cache */

/* Compute a(n) for any n ≥ 0, extending the cache as needed. */
seq_t a(idx_t n)
{
    size_t up_to = (size_t)n;        /* indices fit in size_t for the ranges we handle */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory → return 0 (undefined behavior). */
        cache[0] = 0;                  /* a(0) = 0 */
        cache_sz = 1;
    }

    /* Extend cache up to 'up_to' if necessary. */
    if (up_to >= cache_sz) {
        seq_t *new_cache = (seq_t *)malloc((up_to + 1) * sizeof(seq_t));
        if (new_cache == NULL) return 0;  /* out of memory */

        /* Copy existing entries. */
        for (size_t i = 0; i < cache_sz; ++i) {
            new_cache[i] = cache[i];
        }

        /* Compute new cumulative sums. */
        for (size_t i = cache_sz; i <= up_to; ++i) {
            idx_t t = (idx_t)i;
            unsigned sum = 0;
            while (t > 0) {
                sum += (unsigned)(t & 3ULL);  /* last two bits = base‑4 digit */
                t >>= 2U;
            }
            /* square the digit sum and add to the previous total */
            seq_t square = (seq_t)sum * (seq_t)sum;
            new_cache[i] = new_cache[i - 1] + square;
        }

        /* Replace old cache. */
        free(cache);
        cache     = new_cache;
        cache_sz  = up_to + 1;
    }

    return cache[up_to];
}
