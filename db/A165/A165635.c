/*
 * A165635  Primes of the form (p^2 - 3)/2 where p is also prime.
 * https://oeis.org/A165635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime of the form (p^2 - 3)/2 where p itself is
   prime. It generates primes p incrementally, tests the formula for primality,
   stores the results, and memoises them for fast repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Static memoisation tables */
    static idx_t max_n = 0;        /* number of terms already computed */
    static seq_t *vals   = NULL;     /* array of sequence values      */
    static size_t vals_cap = 0;      /* capacity of vals              */

    static uint64_t *primes   = NULL;/* list of primes found so far   */
    static size_t primes_cap  = 0;   /* capacity of primes array      */
    static size_t primes_cnt  = 0;   /* how many primes stored        */

    static uint64_t candidate = 3;   /* next odd number to test       */

    /* Initialise static structures on first call */
    if (vals == NULL) {
        vals_cap = 64;
        vals = (seq_t*)malloc(vals_cap * sizeof(seq_t));

        primes_cap = 128;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
        primes[primes_cnt++] = 2;        /* keep 2 for divisibility tests */
    }

    /* Extend the cache until we have the required n-terms */
    while (max_n < n) {
        uint64_t p = candidate;
        bool is_p_prime = true;

        /* Test primality of candidate p by trial division using primes list */
        idx_t sq = _floor_isqrt_u64(p);
        for (size_t i = 0; i < primes_cnt && primes[i] <= sq; ++i) {
            if (p % primes[i] == 0) {
                is_p_prime = false;
                break;
            }
        }

        if (is_p_prime) {
            /* Store the new prime */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = p;

            /* Compute q = (p^2 - 3)/2 using an intermediate 128‑bit type to avoid
               overflow of seq_t for small builds. */
            unsigned __int128 qp128 = (unsigned __int128)p * (unsigned __int128)p;
            unsigned __int128 q128  = (qp128 - 3) / 2;
            seq_t q_val = (seq_t)q128;

            /* Check that q fits into seq_t.  If it overflows, skip this term. */
            if ((unsigned __int128)q_val == q128 && is_prime_big(q_val)) {
                /* Ensure capacity of the results array */
                if (max_n >= vals_cap) {
                    vals_cap <<= 1;
                    vals = (seq_t*)realloc(vals, vals_cap * sizeof(seq_t));
                }
                vals[max_n++] = q_val;
            }
        }

        candidate += 2;   /* next odd integer */
    }

    return vals[n - 1];
}
