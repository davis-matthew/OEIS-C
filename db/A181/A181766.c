/*
 * A181766  Numbers k such that 3*k + 7 is not prime.
 * https://oeis.org/A181766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k for which 3*k + 7 is not prime
 * Range:     u8 n <= 190, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                      /* offset is 1, 0 is undefined */
        return 0;

    static seq_t *cache   = NULL;    /* cached values of a(1)…a(cache_len)   */
    static size_t  cache_len = 0;    /* number of cached values               */
    static size_t  cache_cap = 0;    /* current capacity of the cache         */
    static idx_t   next_k   = 1;     /* next integer to test                  */

    /* Extend the cache until it contains a(n) */
    while (cache_len < n) {
        /* Find the next k such that 3*k+7 is not prime */
        while (true) {
            idx_t v = 3 * next_k + 7;
            if (!is_prime(v))
                break;
            next_k++;
        }

        /* Grow the cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)       /* allocation failure – abort */
                exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found value and prepare for the next search */
        cache[cache_len++] = (seq_t)next_k;
        next_k++;
    }

    return cache[n - 1];
}
