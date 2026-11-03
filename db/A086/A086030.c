/*
 * A086030  a(n) = Sum_{i=1..n} C(i+6,7)^3.
 * https://oeis.org/A086030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+6,7)^3 (OEIS A086030, offset 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 24, u128 n <= 204,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 719 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches already computed terms so that any order
   of calls works in O(n) time overall. */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute binomial coefficient C(n,7) by the multiplicative formula.
   n must be ≥ 7.  All intermediate divisions are exact. */
static inline seq_t binom7(idx_t n)
{
    seq_t r = 1;
    for (idx_t k = 1; k <= 7; ++k)
    {
        /* numerator terms: n-7+1 .. n  (i.e. n-6, n-5, ..., n) */
        r = r * (seq_t)(n - 7 + k);
        r = r / (seq_t)k;           /* division is exact */
    }
    return r;
}

/*--------------------------------------------------------------------*/
/*   Cached evaluation of a(n)                                       */
/*--------------------------------------------------------------------*/
static seq_t *cache      = NULL;   /* allocated array, indices 0..len-1 */
static idx_t  cache_len  = 0;      /* number of allocated entries       */
static idx_t  computed   = 0;      /* largest index for which a[] is known */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n<1; sequence starts at 1 */

    /* Allocate/resize the cache so that it contains indices 0..n. */
    if (cache_len <= n)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_len);
        if (!new_cache)            /* allocation failure */
            return (seq_t)0;
        cache = new_cache;
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;   /* initialise new entries to zero */
        cache_len = new_len;
    }

    /* Compute any missing terms from (computed + 1) up to n. */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t c   = binom7(i + 6);       /* C(i+6,7) */
        seq_t term = c * c * c;           /* cube it */
        cache[i] = cache[i - 1] + term;   /* cumulative sum up to i */
    }

    computed = n;          /* mark all needed terms as computed */
    return cache[n];
}
