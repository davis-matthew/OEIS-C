/*
 * A136659  Unsigned third column (k=2) of triangle A136656 divided by 4.
 * https://oeis.org/A136659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/48)*(n+8)*(n+1)*(n+3)!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_limit = 0;   /* size of cache (next free index) */

    /* Ensure cache[0] == 1 (a(0)) */
    if (cache_limit == 0) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – not expected */
        cache[0] = (seq_t)1;
        cache_limit = 1;
    }

    /* Return readily if we have already computed this index */
    if (n < cache_limit) return cache[n];

    /* Grow cache to size n+1 */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* allocation failure – not expected */
    cache = new_cache;

    /* Fill in missing terms using the recurrence:
       a(k+1) = a(k) * ((k+9)*(k+2)*(k+4)) / ((k+8)*(k+1))   */
    for (idx_t i = cache_limit; i <= n; ++i) {
        if (i == 0) continue;   /* already set above */

        idx_t k = i - 1;                     /* k = n for the recurrence */
        seq_t prev   = cache[i - 1];
        seq_t denom1 = (seq_t)(k + 8);
        seq_t denom2 = (seq_t)(k + 1);
        seq_t num1   = (seq_t)(k + 9);
        seq_t num2   = (seq_t)(k + 2);
        seq_t num3   = (seq_t)(k + 4);

        seq_t g;

        /* Reduce denominator parts that divide the previous term */
        g = gcd(prev, denom1);
        prev   /= g;
        denom1 /= g;

        g = gcd(prev, denom2);
        prev   /= g;
        denom2 /= g;

        /* Cancel remaining denominators against numerator factors */
        g = gcd(denom1, num1);
        denom1 /= g;
        num1   /= g;
        g = gcd(denom2, num1);
        denom2 /= g;
        num1   /= g;
        prev *= num1;

        g = gcd(denom1, num2);
        denom1 /= g;
        num2   /= g;
        g = gcd(denom2, num2);
        denom2 /= g;
        num2   /= g;
        prev *= num2;

        g = gcd(denom1, num3);
        denom1 /= g;
        num3   /= g;
        g = gcd(denom2, num3);
        denom2 /= g;
        num3   /= g;
        prev *= num3;

        /* denom1 and denom2 should now be 1; any remaining factors
           would be 1 for all indices that fit in seq_t. */
        cache[i] = prev;
    }

    cache_limit = n + 1;
    return cache[n];
}
