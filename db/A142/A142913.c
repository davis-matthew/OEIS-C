/*
 * A142913  Primes congruent to 43 mod 63.
 * https://oeis.org/A142913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 43 (mod 63)
 * Range:     u8 n <= 1, u16 n <= 132, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Next candidate in the arithmetic progression 43 + 63k */
    static uint64_t next_candidate = 43ULL;

    /* If n is zero, just return 0 (undefined behaviour, but allowed) */
    if (n == 0)
        return (seq_t)0;

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Allocate more space when needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failure -- unlikely */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test current candidate for primality */
        if (is_prime_big((seq_t)next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;

        /* Move to the next number in the residue class */
        next_candidate += 63ULL;
    }

    return cache[n - 1];
}
