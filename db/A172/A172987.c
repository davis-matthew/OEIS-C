/*
 * A172987  Numbers k such that k + 10 and k + 100 are prime.
 * https://oeis.org/A172987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   numbers k such that k + 10 and k + 100 are prime
 * Range:     u8 n <= 33, u16 n <= 2324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;

    if (n == 0)
        return 0; /* undefined, but safe for any accidental call */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n)
    {
        ++last_k; /* candidate k */

        idx_t k10  = last_k + 10;
        idx_t k100 = last_k + 100;

        if (is_prime(k10) && is_prime(k100))
        {
            /* Ensure the cache has space */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)   /* allocation failed */
                    return 0;
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
