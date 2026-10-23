/*
 * A171777  E.g.f.: A(x) = exp( Sum_{n>=1} 2^(n(n-1)/2) * x^n/n ).
 * https://oeis.org/A171777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A171777: a(n) are the coefficients of the egf
   A(x) = exp( Σ_{m≥1} 2^{m(m-1)/2} * x^m / m ).  */
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cached values */
    static seq_t *cache      = NULL;
    static seq_t *l_cache    = NULL; /* 2^{k(k-1)/2} */
    static seq_t *fact_cache = NULL; /* k!  */
    static size_t cache_cap  = 0;     /* number of elements stored */

    if ((size_t)(n + 1u) > cache_cap) {
        /* need to enlarge the caches */
        size_t new_cap = (size_t)(n + 1u);
        cache      = seq_realloc(cache,      cache_cap, new_cap);
        l_cache    = seq_realloc(l_cache,    cache_cap, new_cap);
        fact_cache = seq_realloc(fact_cache, cache_cap, new_cap);

        /* initialise base cases if the cache was empty */
        if (cache_cap == 0) {
            cache[0] = (seq_t)1;
            /* l_cache[0] is unused; initialise for safety */
            l_cache[0] = (seq_t)0;
            fact_cache[0] = (seq_t)1;
            cache_cap = 1;
        }

        /* compute missing l_cache and fact_cache entries */
        for (idx_t k = (idx_t)cache_cap; k <= n; ++k) {
            if (k == 0) continue;           /* unused */
            if (k == 1) {
                l_cache[k] = pow2((idx_t)0); /* 2^0 */
            } else {
                idx_t exp = k * (k - 1) / 2;
                l_cache[k] = pow2(exp);
            }
            fact_cache[k] = fact_cache[k - 1] * (seq_t)k;
        }

        /* compute a(i) for all new indices */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1; /* binom(i-1,0) */
            for (idx_t k = (idx_t)1; k <= i; ++k) {
                seq_t term = cache[i - k];
                term = term * l_cache[k];
                term = term * bin;
                term = term * fact_cache[k - 1];
                sum = sum + term;

                if (k < i) {
                    seq_t numerator = bin * (seq_t)(i - k);
                    bin = numerator / (seq_t)k; /* exact division */
                }
            }
            cache[i] = sum;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
