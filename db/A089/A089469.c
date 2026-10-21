/*
 * A089469  a(n+1) = the n-th term of the n-th binomial transform.
 * https://oeis.org/A089469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = sum_{k=0}^{n} a(k)*binomial(n,k)*n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"              /* seq_t big_pow(seq_t, seq_t) */
#include "../utils/seq_alloc_new_array.h" /* seq_t *seq_alloc_new_array(size_t) */
#include "../utils/free_cache.h"          /* void free_cache(seq_t *) */

/* Compute a(n), offset 0 (a(0)=1) */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache       = NULL;   /* array of terms */
    static size_t cache_size = 0;       /* number of terms stored (highest index +1) */
    static size_t cache_cap  = 0;       /* allocated capacity in terms */

    /* Grow the cache if it does not contain a(n) */
    if (cache_size <= (size_t)n) {
        size_t needed = (size_t)n + 1;         /* at least indices 0..n */
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < needed) new_cap = needed;

        /* Allocate a new array with the larger capacity */
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) abort();

        /* Copy existing values into the new array */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free the old array */
        if (cache)
            free_cache(cache);

        /* Point to the new array and update capacity */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t m = cache_size; m <= n; ++m) {
        if (m == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        idx_t p = m - 1;                 /* parameters of the recurrence */
        seq_t sum   = (seq_t)0;

        seq_t binom  = (seq_t)1;                 /* C(p,0) */
        seq_t power  = big_pow((seq_t)p, (seq_t)p); /* p^p, with 0^0 = 1 */

        for (idx_t k = 0; k <= p; ++k) {
            seq_t term = cache[(size_t)k];        /* a(k)   */
            term = term * binom;                 /* * C(p,k) */
            term = term * power;                 /* * p^(p-k) */
            sum += term;

            /* Prepare binomial and power for next k */
            if (k < p) {
                /* C(p,k+1) = C(p,k) * (p-k) / (k+1) */
                seq_t tmp = binom * (seq_t)(p - k);
                binom = tmp / (seq_t)(k + 1);
                /* p^(p-k-1) = p^(p-k) / p */
                power = power / (seq_t)p;
            }
        }
        cache[(size_t)m] = sum;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
