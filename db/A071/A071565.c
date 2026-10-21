/*
 * A071565  Numbers k such that x^k + x^(k-1) + x^(k-2) + ... + x + 1 is
 *          irreducible over GF(7).
 * https://oeis.org/A071565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A019337(n+1) – 1 * A019337 lists the primes p for which 7
 *            is a primitive root * modulo p. We generate the primes, test the
 *            primitive‑root * property via modular exponentiation, and cache
 *            the values * p‑1. The cache is extended on demand until the
 *            requested * index is available. No hard‑cod...
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   linear_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/linear_sieve.h"

/* ---  Helper: modular exponentiation (uint64_t, uses __int128)  --- */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ---  Helper: check whether 7 is a primitive root mod p   --- */
static bool is_primitive_root_7(idx_t p, const uint32_t *spf)
{
    /* Skip p that divide 7 or are too small */
    if (p == 2 || p == 7)
        return false;

    idx_t m = (idx_t)(p - 1);
    uint32_t last_q = 0;
    while (m > 1) {
        uint32_t q = spf[m];
        if (q == 0)          /* safety, should not happen */
            return false;
        if (q != last_q) {
            if (powmod_uint64(7ULL, (p - 1) / q, p) == 1ULL)
                return false;          /* order divides (p‑1)/q */
            last_q = q;
        }
        m /= q;
    }
    return true;
}

/* ----  Main sequence routine  ---- */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *terms = NULL;      /* cached p-1 values */
    static idx_t  terms_used = 0;    /* number of cached values       */

    if (n == 0)
        return 0;

    /* Extend cache until it contains at least n terms */
    static uint32_t sieve_limit = 100000;   /* will grow as needed          */
    while (terms_used < n) {
        /* Allocate helpers for sieve */
        uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) *
                                            (sieve_limit / 10 + 10));
        uint32_t *spf    = (uint32_t *)malloc(sizeof(uint32_t) *
                                            (sieve_limit + 1));
        if (!primes || !spf) {
            free(primes);
            free(spf);
            return 0;                /* graceful failure */
        }
        /* Perform sieve */
        size_t prime_cnt = 0;
        linear_sieve(primes, &prime_cnt, spf, sieve_limit);

        /* Count how many terms we will get at this limit */
        idx_t new_terms_used = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (p <= 3) continue;          /* skip 2 and 3 */
            if (p == 7) continue;          /* 7 is not a primitive root */
            if (is_primitive_root_7((idx_t)p, spf))
                ++new_terms_used;
        }

        /* Allocate or reallocate terms array */
        seq_t *new_terms = (seq_t *)malloc(sizeof(seq_t) *
                                           (new_terms_used + 5));
        if (!new_terms) {
            free(primes);
            free(spf);
            return 0;
        }

        /* Fill terms */
        idx_t idx = 0;
        for (size_t i = 0; i < prime_cnt && idx < new_terms_used; ++i) {
            uint32_t p = primes[i];
            if (p <= 3) continue;
            if (p == 7) continue;
            if (is_primitive_root_7((idx_t)p, spf))
                new_terms[idx++] = (seq_t)(p - 1);
        }

        /* Replace old cache */
        free(terms);
        terms = new_terms;
        terms_used = idx;

        /* Free helpers */
        free(primes);
        free(spf);

        /* Double sieve limit if still insufficient */
        if (terms_used < n) {
            sieve_limit *= 2;
        }
    }

    /* Return cached value (1‑based index) */
    return terms[n - 1];
}
