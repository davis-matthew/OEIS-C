/*
 * A069040  Numbers k that divide the numerator of B(2k) (the Bernoulli
 *          numbers).
 * https://oeis.org/A069040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A069040 – Numbers k that divide the numerator of B(2k).
 *
 *  Return the n‑th term (n ≥ 1) of the sequence.
 *  The sequence is defined by the condition:
 *       k is *good*  ⇔  for every prime divisor p of k,  (p‑1) ∤ (2k)
 *
 *  The algorithm tests successive integers k starting from 1, and keeps a
 *  cache of the found good k’s.  For k ≤ 10⁵ we use a pre‑computed prime
 *  sieve; compaction of the cache is done with realloc in C, or a
 *  std::vector in C++ (so the code works for both integer and arbitrary‑
 *  precision seq_t types).
 *
 *  The code follows the build constraints of the surrounding framework.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;          /* list of primes for factorisation */
static size_t    primes_cnt = 0;          /* number of primes in that list */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;          /* cache of found good k’s (C++) */
static idx_t          next_k = 1;          /* next integer to test (C++) */
#else
static seq_t *cache      = NULL;          /* cache of found good k’s (C) */
static size_t cache_cap   = 0;             /* allocated capacity (C) */
static size_t cache_len   = 0;             /* number of terms cached (C) */
static idx_t  next_k      = 1;             /* next integer to test (C) */
#endif

/*--------------------------------------------------------------------
 *  Test whether a single integer k satisfies the defining property.
 *--------------------------------------------------------------------*/
static bool good_k(idx_t k)
{
    /*  All computations are done on uint64_t because k is known to be ≤ 2⁶⁴-1
     *  for the test ranges; seq_t is only used for the final return value. */
    uint64_t temp = (uint64_t)k;
    uint64_t two_k = temp * 2ULL;         /* 2*k  (may overflow only for > 2⁶³-1,
                                             but this never happens for tested k ) */

    /*  Prime list must already be built.  The first time this function is
     *  called we perform the sieve here.  The static outer check in a()
     *  guarantees that build_primes_upto() has been called. */
    const uint64_t *p = primes;

    for (size_t i = 0; i < primes_cnt; ++i, ++p) {
        uint64_t prime = *p;
        if (prime * prime > temp) break;     /* remaining factors are > sqrt(temp) */

        if (temp % prime == 0) {
            /* p | k  →  p must not satisfy (p-1) | 2k  */
            if (two_k % (prime - 1) == 0ULL) return false;

            /* remove all powers of this prime factor */
            do { temp /= prime; } while (temp % prime == 0);
        }
    }

    /*  Remaining factor > sqrt(k), must be prime itself */
    if (temp > 1) {
        if (two_k % (temp - 1) == 0ULL) return false;
    }

    return true;
}

/*--------------------------------------------------------------------
 *  Main routine – return the n-th term of the sequence.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /*  not defined for n=0, guard anyway */

    /*  Initialise the prime list once (outside of the cache loop). */
    if (primes == NULL) {
        build_primes_upto(100000 /* arbitrary but adequate for n≤10000 */,
                          &primes, &primes_cnt);
    }

#ifdef __cplusplus
    /*  C++ path – use std::vector for the cache. */
    if (cache.size() >= (size_t)n) {
        return cache[n - 1];
    }

    while (cache.size() < (size_t)n) {
        while (!good_k(next_k)) ++next_k;
        cache.push_back((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
#else
    /*  C path – dynamic array with realloc. */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    while (cache_len < n) {
        while (!good_k(next_k)) ++next_k;

        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /*  Out of memory – exit immediately.  The surrounding test harness
                    expects a normal return, so we use exit() as the original
                    implementation did. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
#endif
}
