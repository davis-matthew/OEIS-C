/*
 * A145476  Primes p such that (19 + p)/2 is prime.
 * https://oeis.org/A145476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p with (p+19)/2 prime
 * Range:     u8 n <= 9, u16 n <= 482, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145476: primes p such that (19 + p)/2 is prime */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* --- static cache of found terms --- */
    static seq_t *cache   = NULL;   /* array of a(k) values */
    static size_t cache_sz   = 0;   /* current number of stored terms */
    static size_t cache_cap   = 0;   /* allocated capacity of cache */
    static idx_t next_q       = 11;  /* current candidate prime q */

    /* Ensure cache has enough capacity to hold at least 'needed' terms */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_sz + 1 > cache_cap) {
        size_t new_cap = cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        /* Find next prime q */
        while (!is_prime(next_q))
            ++next_q;

        /* Candidate p = 2*q - 19 */
        seq_t p = ((seq_t)next_q) * ((seq_t)2) - ((seq_t)19);

        /* If p is prime, store it */
        if (is_prime((idx_t)p)) {
            if (cache_sz >= cache_cap) {   /* resize if needed */
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = p;
        }
        ++next_q; /* move on to the next candidate */
    }

    return cache[n - 1];
}
