/*
 * A140844  Primes of the form 210k + 23.
 * https://oeis.org/A140844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime congruent to 23 modulo 210 (a(1)=23)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* If the requested index is zero, return zero (neither tested nor required). */
    if (n == 0)
        return 0;

    /* Static state to remember all primes found so far. */
    static uint64_t *primes   = NULL;   /* array holding the primes (uint64_t for safety) */
    static size_t   sz        = 0;      /* number of primes stored */
    static size_t   cap       = 0;      /* allocated capacity */
    static idx_t    lastk     = 0;      /* next k to test (k starts at 0 → 23) */

    /* Lazy initialization of the array. */
    if (primes == NULL) {
        cap     = 64;
        primes  = (uint64_t *)malloc(cap * sizeof(uint64_t));
        sz      = 0;
        lastk   = 0;
    }

    /* Generate primes until we have enumerated at least n terms. */
    while (sz < n) {
        uint64_t cand = 210ULL * lastk + 23ULL;  /* candidate of the form 210k+23 */

        /* Test primality using the 64‑bit helper. */
        if (is_prime(cand)) {
            /* Grow the array when needed. */
            if (sz == cap) {
                cap <<= 1;      /* double the capacity */
                uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (tmp == NULL) {           /* allocation failure – abort */
                    free(primes);
                    primes = NULL;
                    sz = 0;
                    cap = 0;
                    return 0;
                }
                primes = tmp;
            }
            primes[sz++] = cand;
        }
        ++lastk;     /* advance k for the next candidate */
    }

    /* Return the n-th prime, cast to seq_t.  If seq_t is smaller, the cast
       will overflow by design – the build limits the use of the function to
       values that fit in seq_t. */
    return (seq_t)primes[n - 1];
}
