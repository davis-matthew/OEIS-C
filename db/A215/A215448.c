/*
 * A215448  a(0)=1, a(1)=0, a(n) = a(n-1) + a(n-2) + Sum_{i=0...n-1} a(i).
 * https://oeis.org/A215448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 2, for n >= 3: a(n) = 3*a(n-1) -
 *            a(n-3) (equivalent to a(n) = a(n-1) + a(n-2) + sum_{i=0}^{n-1}
 *            a(i) ) from OEIS A215448
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values.  Dynamically grow as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute a(n) for any n >= 0, in any order of calls. */
seq_t a(idx_t n)
{
    /* Grow cache to hold at least indices 0..n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – return 0 as sentinel; caller should not
               handle this case for OEIS sequences. */
            return (seq_t)0;
        }

        /* Copy existing data if any */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Compute new values */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else if (i == 1)
                new_cache[i] = (seq_t)0;
            else if (i == 2)
                new_cache[i] = (seq_t)2;
            else
                /* a(i) = 3*a(i-1) - a(i-3) */
                new_cache[i] = (seq_t)((seq_t)3 * new_cache[i-1] - new_cache[i-3]);
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
