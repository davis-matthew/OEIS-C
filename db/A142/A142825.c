/*
 * A142825  Primes congruent to 27 mod 61.
 * https://oeis.org/A142825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 27 modulo 61, with a(1) =
 *            149. The function generates candidates 61*k+27 incrementally and
 *            tests each for primality. Results are cached so that calls in
 *            any order are efficient
 * Range:     u8 n <= 1, u16 n <= 111, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* the next candidate in the progression 27, 88, 149, ... */
    static seq_t candidate = 27;

    if (n == 0)
        return 0;   /* undefined below offset; harmless as calls start at n>=1 */

    /* grow cache until we have at least n primes */
    while (cache_len < n)
    {
        /* advance to next element in the arithmetic progression */
        candidate += 61;

        /* test for primality */
        if (is_prime_big(candidate))
        {
            /* expand cache if necessary */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    abort();   /* allocation failure – terminate */
                }
                cache = tmp;
                cache_cap = new_cap;
            }

            /* store the prime */
            cache[cache_len++] = candidate;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
