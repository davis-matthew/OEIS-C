/*
 * A028071  Expansion of 1/((1-3x)(1-5x)(1-10x)(1-11x)).
 * https://oeis.org/A028071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 293*a(n-2) + 1195*a(n-3) - 1650*a(n-4), *
 *            a(0)=1, a(1)=29, a(2)=548, a(3)=8590 * * Computes the OEIS
 *            sequence A028071. The routine uses an * incremental static cache
 *            so that calls in any order are * supported. Multiplication by
 *            the small constants is performed * via the helper...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Initial terms for n < 4. */
    static const seq_t init_vals[4] = {
        (seq_t)1,
        (seq_t)29,
        (seq_t)548,
        (seq_t)8590
    };

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;         /* number of elements allocated */

    /* Return directly for the trivial first terms. */
    if (n < 4)
        return init_vals[(idx_t)n];

    /* If necessary, grow the cache to accommodate n. */
    if ((idx_t)cache_size <= n) {
        size_t new_cap = (size_t)n + 1;  /* new capacity (elements needed) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;            /* allocation failure – unlikely in tests */
        cache = tmp;

        /* Copy the fixed initial terms into the newly allocated space
         * (if they are not already present). */
        if (cache_size < 4) {
            for (idx_t i = cache_size; i < 4 && (idx_t)i <= n; ++i)
                cache[(size_t)i] = init_vals[(size_t)i];
        }

        /* Compute any missing terms from the first missing index up
         * to n using the recurrence relation. */
        idx_t start = (cache_size > 3) ? cache_size : 4;
        for (idx_t i = start; i <= n; ++i) {
            /* Use mul_small to avoid overflow of intermediate products. */
            seq_t t1 = mul_small(cache[i-1],  29);
            seq_t t2 = mul_small(cache[i-2], 293);
            seq_t t3 = mul_small(cache[i-3],1195);
            seq_t t4 = mul_small(cache[i-4],1650);

            /* Subtract the negative components first to keep
               intermediate values non‑negative where possible. */
            seq_t sub = t2 + t4;
            cache[i] = t1 + t3 - sub;
        }

        cache_size = new_cap;
    }

    return cache[n];
}
