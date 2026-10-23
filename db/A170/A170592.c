/*
 * A170592  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=7·6^(n-1) for 1≤n≤47, and for n≥48
 *            a(n)=3·a(n-48)−∑_{k=1}^{47}a(n−k) (∑ denotes sum of the 47
 *            preceding terms)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* static cache and supporting data */
    static seq_t *cache       = NULL;   /* array of computed terms        */
    static idx_t cache_len    = 0;      /* number of terms in cache       */
    static idx_t cache_cap    = 0;      /* allocated capacity             */
    static seq_t running_sum  = 0;      /* sum of the last 47 computed terms */

    /* Ensure the cache has enough space for n+1 terms */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 48;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure: return 0 */
        cache     = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* (Re)initialize the first 48 terms on the first call */
    if (cache_len == 0)
    {
        /* a(0) = 1 */
        cache[0] = 1;

        /* a(1) .. a(47) : 7 * 6^(n-1) */
        seq_t term = 7;          /* for n = 1 */
        cache[1]   = term;

        seq_t sum47 = term;      /* running sum of a(1..47) */

        for (idx_t i = 2; i <= 47; ++i)
        {
            term = mul_small(term, 6);     /* multiply by 6 */
            cache[i] = term;
            sum47   += term;
        }

        /* store the sum of the first 47 (non‑zero) terms */
        running_sum = sum47;

        cache_len = 48;
    }

    /* If the requested n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Generate terms up to n using the linear recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* a(i) = 3·a(i-48) − running_sum */
        seq_t new_term = mul_small(cache[i - 48], 3);
        new_term      -= running_sum;

        cache[i] = new_term;

        /* Update running_sum: drop a(i-47) and add a(i)   */
        running_sum += new_term;
        running_sum -= cache[i - 47];

        ++cache_len;
    }

    return cache[n];
}
