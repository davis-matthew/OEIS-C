/*
 * A026108  Expansion of 1/((1-2*x)*(1-5*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A026108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(1) = 28 * a(2) = 519 * a(3) = 8078 * a(n) =
 *            28*a(n-1) - 265*a(n-2) + 966*a(n-3) - 1080*a(n-4) (n ≥ 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* allocated cache for computed terms   */
    static size_t cache_cap = 0;         /* capacity of the cache (number of elements) */
    static idx_t cached_up_to = 0;       /* next index that needs to be computed   */

    /* Ensure the cache is large enough to hold up to index n inclusive. */
    if ((idx_t)cache_cap <= n)
    {
        size_t needed = (size_t)n + 1;   /* we need (n+1) elements */
        if (needed == 0)                 /* guard against n == SIZE_MAX */
            needed = 1;

        if (cache_cap < needed)
        {
            seq_t *newp;
            newp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!newp) abort();          /* out of memory */
            cache = newp;
            cache_cap = needed;
        }
    }

    /* Compute values up to n, if not already computed. */
    if (cached_up_to <= n)
    {
        for (idx_t i = cached_up_to; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = 1;
            else if (i == 1)
                cache[1] = 28;
            else if (i == 2)
                cache[2] = 519;
            else if (i == 3)
                cache[3] = 8078;
            else
            {
                /* a(i) = 28*a(i-1) - 265*a(i-2) + 966*a(i-3) - 1080*a(i-4)   */
                seq_t term1 = 28 * cache[i - 1];
                seq_t term2 = 265 * cache[i - 2];
                seq_t term3 = 966 * cache[i - 3];
                seq_t term4 = 1080 * cache[i - 4];
                cache[i] = term1 + term3 - (term2 + term4);
            }
        }
        cached_up_to = n + 1; /* indices 0..n are now computed */
    }

    return cache[n];
}
