/*
 * A207361  Displacement under constant discrete unit surge.
 * https://oeis.org/A207361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(1 + 11*n + 8*n^2)/120
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by the recurrence:
   a(0) = 0
   a(n) = a(n-1) + n^2*(n+1)*(2*n+1)/6
   The result is memoised so each n is computed only once. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the nth term of OEIS A207361. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains the base value a(0). */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();       /* out of memory */
        cache[0] = 0;
    }

    /* Extend the cache if necessary. */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */
        /* Compute new entries starting from the first missing index. */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t ii = (seq_t)i;
            /* term = i^2*(i+1)*(2*i+1)/6 */
            seq_t term = ii * ii;                       /* i^2 */
            term *= (ii + 1);                           /* * (i+1) */
            term *= ((seq_t)2 * ii + 1);                /* * (2*i+1) */
            term /= 6;                                  /* /6 */
            new_cache[i] = new_cache[i - 1] + term;     /* cumulative sum */
        }
        cache_len = new_len;
        cache = new_cache;
    }
    return cache[n];
}
