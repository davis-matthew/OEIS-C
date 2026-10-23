/*
 * A034699  Largest prime power factor of n.
 * https://oeis.org/A034699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest prime power factor of n
 * Range:     u8 n <= 255, u16 n <= 9600, u32 n <= 9600, u64 n <= 9600,
 *            u128 n <= 9600, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest prime power factor of n, supporting
 * arbitrary idx_t and seq_t types supplied by the build.
 * Uses a dynamic prime sieve that grows as necessary.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

/* Static storage for the prime list */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    prime_limit   = 0;

/* Ensure that we have all primes up to at least sqrt(n) + 1. */
static void
_ensure_primes(idx_t n)
{
    idx_t needed = floor_isqrt_u64(n) + 1;   /* +1 to handle perfect squares */

    if (needed > prime_limit) {
        size_t  new_cnt = 0;
        uint64_t *new_pr = NULL;

        /* build_primes_upto allocates an array of primes <= needed. */
        build_primes_upto(needed, &new_pr, &new_cnt);

        free(prime_list);               /* free the old array, if any */
        prime_list   = new_pr;
        prime_count  = new_cnt;
        prime_limit  = needed;
    }
}

seq_t
a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n <= 1) {
        return 1;
    }

    _ensure_primes(n);

    seq_t largest = 1;
    idx_t temp    = n;

    /* Factorise n using the prime list. */
    for (size_t i = 0; i < prime_count &&
                      (idx_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= (seq_t)p;          /* p^e for this prime */
                if (p_pow > largest) {
                    largest = p_pow;
                }
            }
        }
    }

    /* If a prime factor greater than sqrt(n) remains, it appears once. */
    if (temp > 1) {
        seq_t rem = (seq_t)temp;
        if (rem > largest) {
            largest = rem;
        }
    }

    return largest;
}
