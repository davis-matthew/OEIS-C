/*
 * A108472  Numbers n such that 4*prime(n)+prime(n+1) is prime.
 * https://oeis.org/A108472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that 4*prime(n)+prime(n+1) is prime
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Constants for static buffers */
    /* Adjust capacities if higher than 10000 terms are ever needed */
    enum { CACHE_CAP = 150000 };
    enum { PRIMES_CAP = 180000 };

    /* Cache of found terms a(k) */
    static seq_t cache[CACHE_CAP];
    static size_t cache_size = 0;          /* number of terms stored */

    /* Primes storage */
    static uint64_t primes[PRIMES_CAP];
    static size_t primes_size = 0;         /* number of primes stored */
    static uint64_t prime_candidate = 2;   /* current candidate for next prime */

    /* Highest index we have tested for the property */
    static idx_t tested_up_to = 0;

    /* If the requested term is already known, return it */
    if (n > 0 && (size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];

    /* Compute terms until we reach the n-th one */
    while (cache_size < (size_t)n) {
        ++tested_up_to;                   /* next integer to test */

        /* Ensure we have the primes up to tested_up_to+1 (indices are 1‑based) */
        while (primes_size < (size_t)(tested_up_to + 1)) {
            if (is_prime(prime_candidate)) {
                if (primes_size == PRIMES_CAP)
                    abort();               /* buffer overflow unlikely for n<=10000 */
                primes[primes_size++] = prime_candidate;
            }
            ++prime_candidate;
        }

        /* Compute 4*prime(tested_up_to)+prime(tested_up_to+1) */
        uint64_t val = 4ULL * primes[(size_t)(tested_up_to - 1)]
                       + primes[(size_t)tested_up_to];

        /* If result is prime, record the current index */
        if (is_prime(val)) {
            if (cache_size == CACHE_CAP)
                abort();                   /* buffer overflow unlikely for n<=10000 */
            cache[cache_size++] = (seq_t)tested_up_to;
        }
    }

    return cache[(size_t)(n - 1)];
}
