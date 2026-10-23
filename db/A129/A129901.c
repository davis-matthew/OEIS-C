/*
 * A129901  For two consecutive primes p and q the difference 10*q - p is
 *          prime. The first of the pair of primes is listed.
 * https://oeis.org/A129901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 656, u32 n <= 54, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over consecutive primes p and q;
   if 10*q - p is prime, record p.  Return the n‑th recorded prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility headers – they provide the headers for is_prime_big
   and next_prime used in this implementation. */
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Returns the n‑th term of OEIS A129901 (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;     /* cached qualifying primes p     */
    static size_t capacity = 0;      /* allocated capacity of primes   */
    static size_t count   = 0;       /* number of primes stored        */
    static uint64_t last_prime = 0;  /* last prime examined (0 → start) */

    /* Grow the cache until it contains at least n elements. */
    while (count < (size_t)n) {
        /* Find the next two consecutive primes. */
        uint64_t p = next_prime(last_prime);   /* current prime p */
        uint64_t q = next_prime(p);            /* next prime q   */

        /* t = 10 * q - p  (all arithmetic done in seq_t). */
        seq_t t = (seq_t)10 * (seq_t)q - (seq_t)p;

        /* If t is prime, store p in the cache. */
        if (is_prime_big(t)) {
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – return 0 (undefined in OEIS). */
                    return 0;
                }
                primes = tmp;
                capacity = newcap;
            }
            primes[count++] = (seq_t)p;
        }

        /* Prepare for the next iteration. */
        last_prime = p;
    }

    /* Return the requested term (a(1) is at index 0). */
    return primes[(size_t)n - 1];
}
