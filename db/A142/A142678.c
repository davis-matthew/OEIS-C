/*
 * A142678  Primes congruent to 22 mod 57.
 * https://oeis.org/A142678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 22 (mod 57)
 * Range:     u8 n <= 2, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t  *cache     = NULL;  /* array of primes found so far */
    static size_t  cache_len = 0;     /* number of stored primes   */
    static size_t  cache_cap = 0;     /* allocated capacity        */
    static idx_t   last_chk  = 22;    /* last value tested (next is 79) */

    if (n == 0) /* out of range, but we return 0 for safety */
        return 0;

    /* Allocate cache if this is the first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* unrecoverable allocation failure */
    }

    /* Generate further primes until we have at least n of them */
    while (cache_len < n) {
        last_chk += 57;                     /* next candidate in the progression */
        if (is_prime(last_chk)) {           /* check primality */
            if (cache_len >= cache_cap) {   /* grow the array if needed */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len] = (seq_t)last_chk;
            cache_len++;
        }
    }

    return cache[n - 1];   /* a(1) corresponds to cache[0] */
}
