/*
 * A147297  Primes of the form (2k)^2 + 3(2k + 1)^2.
 * https://oeis.org/A147297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form (2k)^2 + 3(2k+1)^2, offset = 1
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 19, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests successive k = 1,2,3,…, keeps a dynamic cache of found primes. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* offset 1 – the first value corresponds to n = 1 */
    if (n == 0) return 0;          /* behaviour for n < 1 is irrelevant */

    static seq_t *cache   = NULL;  /* dynamic array of found primes */
    static idx_t  cache_len  = 0;  /* number of primes stored so far */
    static idx_t  cache_cap  = 0;  /* current capacity of the array */
    static idx_t  next_k     = 1;  /* k to be tested next */

    /* Extend cache until it contains at least n primes */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_k;
        /* (2k)^2 + 3(2k+1)^2  = 16k^2 + 12k + 3 */
        seq_t p = 16 * k * k + 12 * k + 3;

        if (is_prime_big(p))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = seq_alloc((size_t)new_cap);
                if (cache != NULL)
                {
                    tmp = seq_realloc(cache, cache_cap, new_cap);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
        ++next_k;
    }

    return cache[n - 1];
}
