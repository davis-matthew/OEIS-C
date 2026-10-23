/*
 * A154620  Primes p such that 31p+14 is prime.
 * https://oeis.org/A154620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that 31*p+14 is also prime
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A154620: Primes p such that 31p+14 is prime */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;   /* array of found primes p */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t count = 0;      /* number of terms stored */
    static uint64_t current = 2;  /* next candidate prime p to test */

    /* grow cache to at least n elements */
    ensure_capacity(n, &cap, (void **)&cache, sizeof(seq_t));

    /* generate terms until we have at least n */
    while (count < n) {
        /* find next prime p */
        while (!is_prime(current))
            ++current;
        uint64_t p = current;

        /* compute 31*p + 14 */
        uint64_t q = 31ULL * p + 14ULL;

        /* test if q is prime */
        if (is_prime_big(q)) {
            cache[count] = (seq_t)p;
            ++count;
        }
        ++current;
    }

    return cache[n - 1];
}
