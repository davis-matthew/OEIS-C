/*
 * A077612  Number of adjacent pairs of form (even,even) among all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A077612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2) * (floor(n/2) - 1) * (n-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of adjacent (even,even) pairs in all permutations of 1..n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n less than 4 the count is 0. */
    if (n < 4)
        return (seq_t)0;

    idx_t e = n / 2;          /* floor(n/2) */
    if (e < 2)                /* only n≥4 can give a positive product */
        return (seq_t)0;

    /* Static cache of factorials: fact_cache[i] = i! for i <= cache_limit. */
    static seq_t *fact_cache = NULL;
    static idx_t cache_limit = 0;   /* maximum i for which factorial is cached */

    /* Initialise cache if first call. */
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t)); /* size 1 for 0! */
        if (!fact_cache) return (seq_t)0;            /* allocation failure: return 0 */
        fact_cache[0] = (seq_t)1;                   /* 0! = 1 */
        cache_limit = 0;
    }

    /* Ensure we have factorial up to (n-1). */
    if (cache_limit < n - 1) {
        idx_t new_size = n;                       /* need indices 0..(n-1) */
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failure */
        fact_cache = tmp;

        /* Compute missing factorials. */
        for (idx_t i = cache_limit + 1; i <= n - 1; ++i)
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

        cache_limit = n - 1;
    }

    /* Compute floor(n/2) * (floor(n/2)-1) * (n-1)! */
    seq_t prod_pair = (seq_t)e * (seq_t)(e - 1);
    seq_t result   = prod_pair * fact_cache[n - 1];

    return result;
}
