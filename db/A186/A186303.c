/*
 * A186303  a(n) = ( A007522(n)+1 )/2.
 * https://oeis.org/A186303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A007522(n)+1)/2
 * Range:     u8 n <= 14, u16 n <= 2035, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007522(n) is the n‑th prime ≡ 7 (mod 8). */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage of all primes discovered so far                */
    static uint64_t *primes = NULL;
    static size_t   primes_len = 0;
    static size_t   primes_cap = 0;

    /* Static storage of the primes ≡ 7 (mod 8) (the A007522 seq.)   */
    static uint64_t *good   = NULL;
    static size_t   good_len = 0;
    static size_t   good_cap = 0;

    /* Last odd integer tested for primality (next candidate)       */
    static uint64_t last_candidate = 0;

    /* Initialise the prime tables on first call                     */
    if (primes == NULL) {
        primes_cap = 32;
        primes     = (uint64_t *)malloc(primes_cap * sizeof *primes);
        primes[0]   = 2;
        primes_len  = 1;
    }
    if (good == NULL) {
        good_cap = 8;
        good     = (uint64_t *)malloc(good_cap * sizeof *good);
    }
    if (last_candidate == 0) {
        last_candidate = 3;                /* start with first odd */
    }

    /* Ensure we have at least n good primes                           */
    while (good_len < (size_t)n) {
        /* Skip even numbers, although last_candidate is always odd  */
        /* until a prime is found, test for primality                */
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > last_candidate)
                break;
            if (last_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Store new prime */
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof *primes);
            }
            primes[primes_len++] = last_candidate;

            /* If it is ≡7 (mod 8) collect it */
            if ((last_candidate & 7ULL) == 7ULL) {
                if (good_len == good_cap) {
                    good_cap *= 2;
                    good = (uint64_t *)realloc(good,
                                               good_cap * sizeof *good);
                }
                good[good_len++] = last_candidate;
            }
        }

        ++last_candidate;
    }

    /* Return (prime + 1) / 2 using the precision type seq_t         */
    seq_t val = (seq_t)good[n - 1];
    val = val + 1;
    val = val / 2;
    return val;
}
