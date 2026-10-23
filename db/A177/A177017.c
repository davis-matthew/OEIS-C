/*
 * A177017  Primes prime(n) such that prime(n) + prime(n+1) + 1 is also prime.
 * https://oeis.org/A177017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 724, u32 n <= 901, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big, next_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177017: primes p such that p + next_prime(p) + 1 is also prime. 
 *  This function returns the nth term (n >= 1, 1-based indexing).
 *  It uses a static cache to store computed terms and grows it as
 *  necessary.  Each call works independently of the order of calls.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"      /* next_prime(uint64_t) -> uint64_t */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t) -> bool */
#include "../utils/append_candidate.h"/* append_candidate(seq_t **,size_t *,size_t *,seq_t) */
#include "../utils/seq_alloc_array.h"/* seq_alloc_array(size_t) -> seq_t* */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against 0 */
        return (seq_t)0;

    static seq_t *cache = NULL;      /* dynamic array of found primes */
    static size_t cache_len = 0;     /* number of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t last_prime = 0;  /* last prime processed */

    /* Lazy initialisation of cache */
    if (cache == NULL) {
        cache_cap = 1024;           /* arbitrary initial size */
        cache = seq_alloc_array(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p1, p2;

        /* Find the next candidate prime */
        if (last_prime == 0)
            p1 = next_prime(2);
        else
            p1 = next_prime(last_prime + 1);

        /* Find the subsequent prime */
        p2 = next_prime(p1 + 1);

        /* Compute p1 + p2 + 1 and test primality */
        seq_t sum = (seq_t)p1 + (seq_t)p2 + (seq_t)1;
        if (is_prime_big(sum)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p1);
        }

        /* Update state for next iteration */
        last_prime = p1;
    }

    return cache[n - 1];
}
