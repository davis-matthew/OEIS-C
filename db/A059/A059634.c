/*
 * A059634  Primes p such that x^56 = 2 has no solution mod p.
 * https://oeis.org/A059634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, gcd_u64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A059634: primes p such that x^56 = 2 (mod p) has no
   solution.  The function returns the nth such prime, 1‑based.
   It sieves primes, tests the condition
      2^((p-1)/gcd(56,p-1)) ≡ 1 (mod p),
   and memoises results so that calls with arbitrary indices finish
   in O(number of requested primes). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/powmod64.h"
#include "../utils/gcd_u64.h"

/* Euclidean GCD for 64‑bit values */

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined, but return 0 */

    /* ----- static memoisation state ----- */
    static uint64_t *primes = NULL;               /* primes up to current limit */
    static size_t prime_cnt   = 0;                /* number of primes in array   */
    static size_t prime_limit = 0;                /* sieve limit                 */
    static size_t prime_processed = 0;            /* next prime to examine       */

    static seq_t *good_primes = NULL;             /* primes with no 56‑th root    */
    static size_t good_cnt  = 0;                  /* number of good primes stored */
    static size_t good_cap  = 0;                  /* capacity of the array        */

    /* ----- initialise sieve on first call ----- */
    if (!primes) {
        prime_limit = 200000;                     /* initial bound, generous      */
        build_primes_upto((idx_t)prime_limit, &primes, &prime_cnt);
    }

    /* ----- generate good primes until we reach the desired index ----- */
    while (good_cnt < n) {
        /* process remaining primes in the current sieve range */
        while (prime_processed < prime_cnt && good_cnt < n) {
            uint64_t p = primes[prime_processed++];
            if (p <= 2) continue;                /* 2 is a trivial root of 2^56 */

            uint64_t g   = gcd_u64(56ULL, p - 1ULL);
            uint64_t exp = (p - 1ULL) / g;
            uint64_t val = powmod64(2ULL, exp, p);

            if (val != 1ULL) {                    /* no solution exists */
                /* grow the array if necessary */
                if (good_cnt >= good_cap) {
                    size_t new_cap = (good_cap ? good_cap << 1 : 16);
                    seq_t *tmp = (seq_t *)realloc(good_primes, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    good_primes = tmp;
                    good_cap    = new_cap;
                }
                good_primes[good_cnt++] = (seq_t)p;
            }
        }

        /* if still not enough good primes, expand the sieve limits */
        if (good_cnt < n) {
            size_t old_cnt = prime_cnt;           /* primes processed so far */

            /* double the sieve limit */
            prime_limit <<= 1;
            free(primes);
            build_primes_upto((idx_t)prime_limit, &primes, &prime_cnt);

            /* resume processing after the previously processed primes */
            prime_processed = old_cnt;
        }
    }

    /* ----- return the requested term ----- */
    return good_primes[n - 1];
}
