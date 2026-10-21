/*
 * A081367  E.g.f.: exp(2*x)/sqrt(1-2*x).
 * https://oeis.org/A081367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*a(n-1) - 4*(n-1)*a(n-2), with a(0)=1, a(1)=3.
 *            This follows from a(n) = Sum_{k=0..n} A046716(n,k)*2^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for dynamic allocation */
#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Compute a(n) using the recurrence with memoization. */
seq_t a(idx_t n)
{
    /* Allocate enough space for terms up to n */
    if (cache_size <= n)
    {
        /* New size */
        idx_t new_size = n + 1U;
        /* Reallocate cache */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)   /* Allocation failed – abort */
            abort();
        cache      = new_cache;
        cache_size = new_size;

        /* Initialize base cases if not already set */
        if (cache_size > 0)
        {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
            if (cache_size > 1)
                cache[1] = (seq_t)3;      /* a(1) = 3 */
        }

        /* Compute all new terms that are needed */
        for (idx_t i = 2; i <= n; ++i)
        {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t term  = ((seq_t)(2 * i + 1)) * prev1
                          - ((seq_t)(4 * (i - 1))) * prev2;
            cache[i] = term;
        }
    }

    return cache[n];
}
