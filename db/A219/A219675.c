/*
 * A219675  Starting with a(0)=0, a(n) = 1 + the sum of the digital sums of
 *          a(0) through a(n-1).
 * https://oeis.org/A219675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219675: a(0) = 0;
   for n ≥ 1, a(n) = a(n‑1) + digsum(a(n‑1)),
   where digsum(x) is the sum of decimal digits and digsum(0) = 1. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* Helper that uses the library digit_sum but treats 0 as having digit sum 1. */
static seq_t digit_sum_zero(seq_t x)
{
    return (x == (seq_t)0) ? (seq_t)1 : digit_sum(x);
}

/* Cached values of the sequence.  cache_cap is the allocated capacity,
   cache_max is the highest index that has been computed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max  = 0;

seq_t a(idx_t n)
{
    /* initialise cache with a(0) = 0 on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;         /* allocation failure */
        cache[0] = (seq_t)0;
        cache_max = 0;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n (if not already computed) */
    if (n > cache_max) {
        for (idx_t i = cache_max + (idx_t)1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t ds   = digit_sum_zero(prev);
            cache[i] = prev + ds;
        }
        cache_max = n;
    }

    return cache[n];
}
