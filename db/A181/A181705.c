/*
 * A181705  Numbers of the form 2^(t-1)*(2^t-9), where 2^t-9 is prime.
 * https://oeis.org/A181705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form 2^t-9 and return the nth term */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_t = 4;          /* next t to test */

    /* ensure cache has enough space for up to n terms */
    while (cache_len < n)
    {
        seq_t p_minus = pow2(next_t) - (seq_t)9;
        if (is_prime_big(p_minus))
        {
            /* compute the term 2^(t-1)*(2^t-9) */
            seq_t term = pow2(next_t - 1) * p_minus;
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = term;
        }
        ++next_t;
    }
    return cache[n - 1];
}
