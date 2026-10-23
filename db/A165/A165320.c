/*
 * A165320  Primes p where neither the number of divisors of p+1 nor the
 *          number of divisors of p-1 is a power of 2.
 * https://oeis.org/A165320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_power_of_two, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165320: primes p where neither the number of divisors of p+1
   nor the number of divisors of p-1 is a power of 2.  The function
   returns the n‑th such prime (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Static state – caches the primes that have been found so far. */
    static seq_t *primes = NULL;         /* array of found primes          */
    static size_t   count = 0;           /* number of primes stored        */
    static size_t   capacity = 0;        /* allocated capacity             */
    static idx_t   next_candidate = 3;   /* next odd number to test       */

    /* Generate new primes until we have at least n of them. */
    while (count < n) {
        /* Find the next prime. */
        while (!is_prime(next_candidate)) {
            next_candidate += 2;                 /* skip even numbers       */
        }
        idx_t p = next_candidate;                 /* p is a prime             */
        next_candidate += 2; /* advance for next search */

        /* Count divisors of p-1 and p+1. */
        uint64_t d1 = divisor_count_u64((uint64_t)(p - 1));
        uint64_t d2 = divisor_count_u64((uint64_t)(p + 1));

        /* If either divisor count is a power of two, skip p. */
        if (is_power_of_two((seq_t)d1) || is_power_of_two((seq_t)d2)) {
            continue;
        }

        /* Append p to the cache. */
        if (count == capacity) {
            size_t new_cap = capacity ? capacity * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – terminate program. */
                abort();
            }
            primes = tmp;
            capacity = new_cap;
        }
        primes[count++] = (seq_t)p;
    }

    /* Return the requested term.  The array is 0‑based, so use n-1. */
    return primes[n - 1];
}
