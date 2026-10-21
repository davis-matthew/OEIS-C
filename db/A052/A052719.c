/*
 * A052719  Expansion of e.g.f. (1 - 2*x*sqrt(1-4*x))*(1 - sqrt(1-4*x))/4.
 * https://oeis.org/A052719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=a(1)=a(2)=0, a(3)=6 ; a(n+2) = (2+5*n)*a(n+1) +
 *            (6+2*n-4*n*n)*a(n) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence uses only integer arithmetic.  The implementation
   keeps a static cache so that calls arrive in any order.  We use
   the helper ensure_cache() to allocate or grow the array safely for
   arbitrary‑precision types.  */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* current capacity of the array */

    /* Initialise cache with the first four known terms (offset 0). */
    if (cache_len == 0) {
        ensure_cache(4, &cache, &cache_cap);   /* allocates at least 4 entries */
        cache[0] = 0;       /* a(0) */
        cache[1] = 0;       /* a(1) */
        cache[2] = 0;       /* a(2) */
        cache[3] = 6;       /* a(3) */
        cache_len = 4;
    }

    /* Expand cache if the requested index is beyond the current length. */
    if (n + 1 > cache_len) {
        /* make sure the array can hold indices up to n */
        ensure_cache(n + 1, &cache, &cache_cap);

        /* Compute new terms using the recurrence.  We already have
           values up to cache_len-1.  The recurrence is valid for i ≥ 4. */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* use k = i-2 for the coefficients of a(i-1) and a(i-2)   */
            idx_t k = i - 2;
            seq_t coeff1 = (seq_t)2 + (seq_t)5 * (seq_t)k;
            seq_t coeff2 = (seq_t)6 + (seq_t)2 * (seq_t)k
                            - (seq_t)4 * (seq_t)k * (seq_t)k;
            cache[i] = coeff1 * cache[i - 1] + coeff2 * cache[i - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
