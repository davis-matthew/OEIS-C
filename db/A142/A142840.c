/*
 * A142840  Primes congruent to 42 mod 61.
 * https://oeis.org/A142840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes p ≡ 42 (mod 61) – a(n) is the nth such prime. */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached primes */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t next_k = 1;     /* current k such that p = 61*k+42 */

    if (n < 1) return (seq_t)0;     /* out of range */

    /* grow cache until we have at least n primes */
    while (cache_size < n)
    {
        uint64_t p = 61ULL * next_k + 42ULL;
        next_k++;

        if (p < 2) continue;                      /* skip numbers < 2 */

        /* use provided prime test for 64‑bit integers */
        if (is_prime((idx_t)p))
        {
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) return (seq_t)0;   /* allocation failure, return 0 */
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
