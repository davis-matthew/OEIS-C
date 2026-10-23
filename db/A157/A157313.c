/*
 * A157313  G.f.: exp( Sum_{n>=1} a(n)*x^n/n ) = Product_{n>=1} 1/(1 -
 *          a(n-1)*x^n).
 * https://oeis.org/A157313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * a(d-1)^(n/d) (a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initialise cache to hold a(0) */
    if (cache_cap == 0) {
        cache     = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0]  = (seq_t)1;           /* a(0) = 1 */
        cache_cap = 1;
    }

    /* if the requested value has already been computed, use it */
    if (n < cache_cap) {
        return cache[n];
    }

    /* extend the cache array up to index n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute all missing values */
    for (idx_t i = cache_cap; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* iterate over all divisors d of i */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor d */
                idx_t exp1 = i / d;
                seq_t term1 = big_pow(cache[d - 1], (seq_t)exp1);
                sum += (seq_t)d * term1;

                /* paired divisor i/d */
                idx_t d2 = i / d;
                if (d2 != d) {
                    idx_t exp2 = i / d2;   /* equals d */
                    seq_t term2 = big_pow(cache[d2 - 1], (seq_t)exp2);
                    sum += (seq_t)d2 * term2;
                }
            }
        }

        cache[i] = sum;
    }

    cache_cap = new_cap;
    return cache[n];
}
