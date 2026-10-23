/*
 * A120243  Numbers k such that {k*sqrt(2)} < 1/2, where { } = fractional
 *          part.
 * https://oeis.org/A120243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) for OEIS A120243.
 *
 *  a(n) = the nth positive integer k such that the fractional part of
 *         k * sqrt(2) is less than 1/2.
 *
 *  The algorithm tests integers k in increasing order, using only
 *  integer arithmetic.  For each k it computes
 *
 *          m = floor( sqrt( 2 * k^2 ) )
 *
 *  and checks the equivalent integer inequality
 *
 *          2 * k^2 <= m * (m + 1)
 *
 *  which is true exactly when {k * sqrt(2)} < 1/2.  The relation
 *  follows from  sqrt(2*k^2) < m + 1/2  <=>  8*k^2 < (2m+1)^2
 *  and reducing to integer form.
 *
 *  The sequence values are cached so that repeated calls with any order
 *  of n have amortised O(1) time per value after the required terms
 *  have been generated.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_k     = 1;   /* next integer k to test */

    /* Grow the cache until it contains at least n terms */
    if (cache_len < (size_t)n)
    {
        /* Ensure sufficient capacity */
        if (cache_cap < (size_t)n)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < (size_t)n)
                new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we have n of them */
        while (cache_len < (size_t)n)
        {
            idx_t k = next_k;
            seq_t k_seq = (seq_t)k;
            seq_t k_sq  = k_seq * k_seq;          /* k^2  */
            seq_t two   = (seq_t)2;
            seq_t m     = floor_sqrt(two * k_sq); /* floor(sqrt(2*k^2)) */

            /* Test the integer inequality 2*k^2 <= m*(m+1) */
            if ( (k_sq * two) <= m * (m + (seq_t)1) )
            {
                cache[cache_len++] = k_seq;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
