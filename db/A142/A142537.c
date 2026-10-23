/*
 * A142537  Primes congruent to 7 mod 53.
 * https://oeis.org/A142537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime congruent to 7 modulo 53 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; for n==0 return a dummy 0. */
    if (n == 0)
        return 0;

    /* Persistent state: cached primes, array capacity, and next candidate. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of cache   */
    static size_t cache_len = 0;   /* number of primes stored so far */
    static idx_t next_candidate = 7; /* first number ≡ 7 (mod 53) */

    /* Generate primes until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (is_prime(next_candidate))               /* test primality */
        {
            /* Ensure there is room for another prime. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                       /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate; /* store the prime */
        }
        next_candidate += 53;                      /* next candidate ≡ 7 (mod 53) */
    }

    return cache[n - 1];                            /* zero‑based index into cache */
}
