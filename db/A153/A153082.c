/*
 * A153082  Numbers k such that 2*k + 13 is not prime.
 * https://oeis.org/A153082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2*k + 13 is not prime. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Static cache of values already computed. */
    static seq_t  *cache   = NULL;   /* array of seq_t values */
    static size_t  cache_len = 0;    /* number of values stored */
    static size_t  cache_cap = 0;    /* current allocation capacity */
    static idx_t   next_k   = 1;     /* smallest k not yet examined */

    /* Ensure we have at least n terms cached. */
    while (cache_len < n) {
        /* Find the next k for which 2*k+13 is composite. */
        while (is_prime_small(((seq_t)next_k) * (seq_t)2 + (seq_t)13))
            ++next_k;

        /* Grow the cache if necessary – double the capacity. */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* insufficient memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found value. */
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
