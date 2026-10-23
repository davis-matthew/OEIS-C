/*
 * A105875  Primes for which -3 is a primitive root.
 * https://oeis.org/A105875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap, factorize_uint64, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p such that -3 is a primitive
   root modulo p (OEIS A105875).  The function uses a sieve of
   Eratosthenes‑like prime generation and stores the qualified primes
   in a growing static array.  No hard‑coded table is used. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/modpow_uint64.h"
#include "../utils/ensure_cap.h"
#include "../utils/factorize_uint64.h"

/* trial division primality test using the list of known primes */
static bool is_prime_candidate(uint64_t x, size_t prime_cnt, const uint64_t *primes)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;          /* p > sqrt(x) */
        if (x % p == 0) return false;
    }
    return true;
}

/* test whether -3 is a primitive root modulo p (p must be prime) */
static bool good_prime(uint64_t p,
                       size_t prime_cnt,
                       const uint64_t *primes)
{
    if (p == 2) return true;       /* 2 is always included */
    if (p % 3 == 0) return false;  /* 3 never works */
    if (p % 6 != 5) return false;  /* necessary condition */

    uint64_t m = p - 1;
    uint64_t fac[32];
    size_t fac_cnt;
    factorize_uint64(m, fac, &fac_cnt, prime_cnt, primes);

    uint64_t base = p - 3;         /* -3 mod p for p > 3 */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        uint64_t exp = m / q;
        if (modpow_uint64(base, exp, p) == 1) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0, primes_cnt = 0;
    static uint64_t *good_primes = NULL;
    static size_t good_cap = 0, good_cnt = 0;
    static uint64_t next_candidate = 3;

    if (!initialized) {
        /* prime list starts with 2 */
        ensure_cap(&primes_cap, &primes_cnt, &primes);
        primes[primes_cnt++] = 2;

        /* first good prime is also 2 */
        ensure_cap(&good_cap, &good_cnt, &good_primes);
        good_primes[good_cnt++] = 2;

        initialized = true;
    }

    /* generate until we have at least n good primes */
    while (good_cnt < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;                   /* skip even numbers */

        if (!is_prime_candidate(p, primes_cnt, primes))
            continue;                          /* not a prime */

        /* add new prime to prime list */
        ensure_cap(&primes_cap, &primes_cnt, &primes);
        primes[primes_cnt++] = p;

        if (good_prime(p, primes_cnt, primes)) {
            ensure_cap(&good_cap, &good_cnt, &good_primes);
            good_primes[good_cnt++] = p;
        }
    }

    return (seq_t)good_primes[n - 1];
}
