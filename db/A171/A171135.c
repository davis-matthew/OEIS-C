/*
 * A171135  Smallest prime p such that p + 2n is prime or semiprime.
 * https://oeis.org/A171135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p + 2n is prime or semiprime
 * Range:     u8 n <= 4964, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/is_prime_big.h"

/* Limit for sieve and smallest‑prime‑factor array */
#define SIEVE_LIMIT 1000000U

static bool initialized = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t *spf = NULL;

/* Check whether a number is prime or semiprime using the precomputed sieve.
 * For numbers larger than the sieve limit a fallback trial division is used. */
static bool is_prime_or_semiprime(seq_t val)
{
    idx_t uval = (idx_t)val;
    if (uval <= SIEVE_LIMIT) {
        idx_t spf_val = spf[uval];
        if (spf_val == uval) return true;   /* prime */

        idx_t tmp = uval;
        size_t cnt = 0;
        while (tmp > 1) {
            tmp /= spf[tmp];
            ++cnt;
            if (cnt > 2) return false;      /* more than 2 prime factors */
        }
        return cnt == 2;                     /* exactly 2 prime factors */
    } else {
        /* Sieve not large enough – use trial division with primes. */
        if (is_prime_big(val)) return true;

        uint64_t x = (uint64_t)val;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > x) break;
            if (x % p == 0) {
                uint64_t q = x / p;
                return is_prime_big((seq_t)q);
            }
        }
        return true;  /* no divisor found → prime */
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Build primes up to SIEVE_LIMIT and smallest‑prime‑factor array. */
        build_primes_upto((idx_t)SIEVE_LIMIT, &primes, &prime_cnt);
        build_spf((idx_t)SIEVE_LIMIT, &spf);
        initialized = true;
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t val = (seq_t)p + ((seq_t)2 * (seq_t)n);
        if (is_prime_or_semiprime(val))
            return (seq_t)p;
    }

    /* In theory this point is never reached because such a prime always
     * exists. Return 0 as a safe fallback. */
    return 0;
}
