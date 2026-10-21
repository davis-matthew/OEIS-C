/*
 * A059500  Primes p such that both q=(p-1)/2 and 2p + 1 = 4q + 3 are
 *          composite numbers. Intersection of A059456 and A053176.
 * https://oeis.org/A059500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 5326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059500 – primes p such that (p‑1)/2 and 2p+1 are both composite.
   The implementation stores discovered terms in a dynamic array so that
   subsequent calls to a(n) act in O(1) time for already computed indices. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Persistent storage of the sequence terms. */
    static seq_t *cache = NULL;   /* array of found primes            */
    static idx_t cached = 0;      /* number of stored primes          */
    static idx_t cap    = 0;      /* current allocation capacity      */
    static idx_t next   = 3;      /* next odd candidate to test (idx_t
                                   works for all builds, casting to uint64_t when
                                   calling the primality test)              */

    /* If the requested term is already cached, return immediately. */
    if (n <= cached) {
        return cache[n - 1];
    }

    /* Initialise the cache on the first call. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Keep generating primes until we have at least n terms. */
    while (cached < n) {
        /* Skip non‑primes. */
        while (!is_prime((uint64_t)next)) {
            next += 2;
        }

        uint64_t p = (uint64_t)next;
        uint64_t q = (p - 1) / 2;  /* (p‑1)/2   */
        uint64_t r = 2 * p + 1;    /* 2p+1      */

        bool q_comp = (q >= 2) && !is_prime(q);
        bool r_comp = !is_prime(r);

        if (q_comp && r_comp) {
            /* Store the prime. */
            if (cached == cap) {
                cap <<= 1; /* double capacity */
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cached++] = (seq_t)p;
        }

        next += 2;
    }

    return cache[n - 1];
}
