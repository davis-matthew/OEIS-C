/*
 * A235743  Primes p(k) such that p(k) + p(k+3) = p(k+1) + p(k+2) + 4.
 * https://oeis.org/A235743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of primes p(k) such that p(k)+p(k+3)=p(k+1)+p(k+2)+4
 *            (OEIS A235743). The function generates the required primes on
 *            demand and memoises them for future calls
 * Range:     u8 n <= 8, u16 n <= 524, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* assumes next_prime(uint64_t) returns next prime >= start */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* capacity of cache */
    static size_t size = 0;     /* number of stored terms */

    /* static state for prime generation */
    static uint64_t  last_prime = 0;   /* last prime generated */
    static seq_t    p0 = 0, p1 = 0, p2 = 0; /* last three primes */

    /* initialise if first call */
    if (size == 0) {
        /* fetch first three primes */
        last_prime = 0;
        p0 = (seq_t)next_prime(last_prime + 1);
        last_prime = (uint64_t)p0;
        p1 = (seq_t)next_prime(last_prime + 1);
        last_prime = (uint64_t)p1;
        p2 = (seq_t)next_prime(last_prime + 1);
        last_prime = (uint64_t)p2;
    }

    /* grow cache as needed */
    while (size < n) {
        /* generate next prime */
        uint64_t p3 = next_prime(last_prime + 1);

        /* test the quadruplet condition */
        if ((seq_t)p0 + (seq_t)p3 == (seq_t)p1 + (seq_t)p2 + (seq_t)4) {
            /* append p0 to cache */
            if (size == cap) {
                /* increase capacity: double or set to 16 if zero */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out of memory: return zero (undefined behaviour otherwise) */
                    abort();
                }
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = p0;
        }

        /* shift window of last three primes */
        p0 = p1;
        p1 = p2;
        p2 = (seq_t)p3;
        last_prime = p3;
    }

    return cache[n - 1];
}
