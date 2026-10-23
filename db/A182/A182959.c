/*
 * A182959  Expansion of o.g.f. 2*(1+x)^2/(1-2*x+sqrt(1-8*x)).
 * https://oeis.org/A182959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 a(n) = 2^n * ( 2*C_n + C_{n-1} ) for n >= 1
 *            where C_k are the Catalan numbers
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"      // provides seq_t pow2(idx_t exp)
#include "../utils/catalan.h"   // provides seq_t catalan(idx_t k)

static seq_t *cache = NULL;           // memoized a(i) values
static size_t cache_cap = 0;          // capacity of cache[]
static idx_t  cache_max = 0;          // highest index computed
static seq_t  last_catalan = (seq_t)1; // C_{cache_max}
static seq_t  pow2_last    = (seq_t)1; // 2^{cache_max}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Grow cache if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            // out of memory
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new values up to n, starting from cache_max + 1. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* pow2_last = 2^i */
        pow2_last += pow2_last;                  // multiply by 2

        /* C_i = C_{i-1} * (4*i-2)/(i+1) */
        seq_t numerator   = (seq_t)(4 * i - 2);
        seq_t newC        = (last_catalan * numerator) / (i + 1);

        /* a_i = 2^i * (2*C_i + C_{i-1}) */
        seq_t sum = newC + newC;                 // 2*C_i
        sum += last_catalan;                     // + C_{i-1}
        seq_t val = pow2_last * sum;

        if (i == 1)                            // subtract the 1 for a(1)
            val -= (seq_t)1;

        cache[i] = val;

        /* Update state for next iteration */
        last_catalan = newC;
    }

    cache_max = n;
    return cache[n];
}
