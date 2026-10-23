/*
 * A106077  Primes p such that 5*p + 2 and 2*p + 5 are primes.
 * https://oeis.org/A106077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that both 5p+2 and 2p+5 are primes. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p with 5p+2 and 2p+5 also prime. */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;   /* pointer to array of terms  */
    static size_t cache_sz   = 0;      /* number of terms currently stored  */
    static size_t cache_cap  = 0;      /* current capacity of the array      */
    static uint64_t next_odd = 3;      /* next odd candidate to test         */

    /* enlarge the cache when necessary */
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* the environment guarantees enough memory; abort on failure */
            abort();
        }
        for (size_t i = 0; i < cache_sz; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_sz < n) {
        uint64_t p = next_odd;
        /* search for the next prime satisfying the conditions */
        for (;;) {
            if (!is_prime(p)) {
                p += 2;
                continue;
            }
            /* 5p+2 and 2p+5 may overflow 64 bits for very large p,
               but for the expected range they fit inside 64 bits.  */
            uint64_t q1 = (uint64_t)((unsigned __int128)5 * p + 2);
            uint64_t q2 = (uint64_t)((unsigned __int128)2 * p + 5);
            if (is_prime(q1) && is_prime(q2)) {
                break;       /* p is the next term */
            }
            p += 2;
        }
        /* store the found prime */
        cache[cache_sz++] = (seq_t)p;
        next_odd = p + 2;   /* search starts after the found prime */
    }

    return cache[n - 1];   /* convert from 1‑based OEIS index */
}
