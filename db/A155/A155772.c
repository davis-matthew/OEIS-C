/*
 * A155772  Primes p such that 2*p^2+2*p-41 is a prime.
 * https://oeis.org/A155772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 2*p^2+2*p-41 is a prime
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches for primes p until the nth qualifying prime is found. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next odd candidate to test; 3 is the first odd prime >2 */
    static uint64_t next_candidate = 3;

    if (n == 0)          /* offset starts at 1; not used in tests */
        return 0;

    /* Grow the cache until it holds at least n elements */
    while (cache_len < (size_t)n)
    {
        uint64_t p = next_candidate;

        /* Only primes can satisfy the condition */
        if (is_prime((idx_t)p))
        {
            seq_t p_seq = (seq_t)p;
            seq_t val   = 2 * p_seq * p_seq + 2 * p_seq - 41;

            if (isPrimeGeneric(val))
            {
                /* store the qualifying prime in the cache */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();          /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p_seq;
            }
        }

        /* Advance to the next odd candidate */
        next_candidate += 2;
    }

    return cache[n - 1];
}
