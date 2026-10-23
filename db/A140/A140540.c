/*
 * A140540  Primes of form 17*n - 3.
 * https://oeis.org/A140540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 17*n - 3 (i.e., primes ≡ 31 mod 34)
 * Range:     u8 n <= 2, u16 n <= 319, u32 n <= 388, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of found primes, growing as needed */
    static seq_t *cache      = NULL;   /* array of primes found  */
    static size_t cache_len       = 0;   /* number of primes stored */
    static size_t cache_cap       = 0;   /* allocated capacity          */
    static idx_t next_k            = 2;   /* next k to be tested (k≥2)   */

    while (cache_len < n)
    {
        /* Candidate of the required form */
        seq_t p = 17 * (seq_t)next_k - (seq_t)3;

        if (is_prime_big(p))           /* test primality */
        {
            /* grow cache if full */
            if (cache_cap <= cache_len)
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

            cache[cache_len++] = p;    /* store found prime */
        }
        ++next_k;    /* move to next k */
    }
    return cache[n - 1];   /* return the n-th prime */
}
