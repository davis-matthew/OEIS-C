/*
 * A165896  a(n) =
 *          (a(n-1)^2+a(n-2)^2+a(n-3)^2+a(n-1)*a(n-2)+a(n-1)*a(n-3)+a(n-2)*a(n-3))/a(n-4)
 *          with four initial ones.
 * https://oeis.org/A165896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2+a(n-2)^2+a(n-3)^2+
 *            a(n-1)*a(n-2)+a(n-1)*a(n-3)+a(n-2)*a(n-3))/a(n-4) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"   /* Provides seq_t *alloc_seq(size_t n) */

seq_t a(idx_t n)
{
    /* Static cache holds already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* If the value is already cached, return it immediately */
    if (n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* Expand the cache to accommodate index n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = alloc_seq(new_size);
    /* Copy existing values (for indices < cache_size) */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Compute any new terms that are not yet cached */
    for (size_t i = cache_size; i < new_size; ++i)
    {
        if (i <= 3)
        {
            new_cache[i] = (seq_t)1;
            continue;
        }

        /* Using 64‑bit index arithmetic for safety with the
           index type (idx_t) which is uint64_t. */
        seq_t a1 = new_cache[i-1];
        seq_t a2 = new_cache[i-2];
        seq_t a3 = new_cache[i-3];
        seq_t a4 = new_cache[i-4];

        /* Compute the numerator in the recurrence */
        seq_t num = a1*a1 + a2*a2 + a3*a3
                     + a1*a2 + a1*a3 + a2*a3;

        new_cache[i] = num / a4;
    }

    /* Replace the old cache with the new one and update its size */
    cache = new_cache;
    cache_size = new_size;

    return cache[(size_t)n];
}
