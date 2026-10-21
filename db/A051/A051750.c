/*
 * A051750  Primes whose cubes lack zeros.
 * https://oeis.org/A051750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth prime p such that the decimal digits of p³
 *            contain no zero. The function generates primes one by one, tests
 *            the cube for zeros and stores the qualifying primes in a static
 *            array so that later calls are O(1). The array is large enough to
 *            hold the first 200 000 terms, which is far m...
 * Range:     u8 n <= 33, u16 n <= 1939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* Static cache of found terms.  The capacity is chosen to be large
       enough for all practical test cases. */
    static seq_t cache[200000] = {0};   /* zero‑initialized elements */
    static size_t cache_len = 0;        /* number of cached terms      */
    static uint64_t candidate = 2;      /* current integer to test     */

    /* Grow the cache until it contains the n‑th term. */
    while (cache_len < n) {
        if (is_prime((idx_t)candidate)) {
            /* Compute the cube using 64‑bit arithmetic – the cube of the
               10 000th prime (104 729) is < 2¹⁶³, so 64 bits are safe. */
            uint64_t cube = (uint64_t)candidate * candidate * candidate;

            /* Test the cube for a zero digit. */
            bool has_zero = false;
            uint64_t t = cube;
            do {
                if ((t % 10) == 0) { has_zero = true; break; }
                t /= 10;
            } while (t);

            if (!has_zero) {
                /* Store the prime in the cache.  The array is large enough
                   that we never exceed its bounds. */
                cache[cache_len] = (seq_t)candidate;
                cache_len++;
            }
        }
        candidate++;   /* advance to next integer */
    }

    return cache[n - 1];
}
