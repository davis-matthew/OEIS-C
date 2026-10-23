/*
 * A191609  Primes modulo which the multiplicative orders of 2 and 3 are
 *          equal.
 * https://oeis.org/A191609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that ord_p(2) == ord_p(3). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external helpers provided by the build system */
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* modular multiplication using 128‑bit type for 64‑bit moduli */
static inline uint64_t mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* modular exponentiation (fast binary powering) */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mul64(result, base, mod);
        base = mul64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* multiplicative order of a modulo the prime p */
static uint64_t order_of(uint64_t a, uint64_t p)
{
    uint64_t m = p - 1;
    uint64_t order = m;

    /* factor m: primes and exponents are stored in temporary arrays */
    uint64_t fac_pr[64];
    uint64_t fac_ex[64];
    size_t fac_cnt = 0;
    factor_u64(m, fac_pr, fac_ex, &fac_cnt);

    for (size_t i = 0; i < fac_cnt; ++i)
    {
        uint64_t q = fac_pr[i];
        while (order % q == 0 && powmod64(a, order / q, p) == 1)
        {
            order /= q;
        }
    }
    return order;
}

/* sequence function with static memoisation */
seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static uint64_t next_candidate = 5;          /* next prime to test */
    static uint64_t *cache = NULL;               /* cached primes */
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;

    if (!initialized)
    {
        cache_cap = 64;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
        initialized = true;
    }

    /* grow cache until we have the requested term */
    while (cache_cnt < (size_t)n)
    {
        /* search for the next prime */
        if (is_prime(next_candidate))
        {
            uint64_t ord2 = order_of(2ULL, next_candidate);
            uint64_t ord3 = order_of(3ULL, next_candidate);
            if (ord2 == ord3)
            {
                /* Ensure capacity */
                if (cache_cnt >= cache_cap)
                {
                    cache_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
                    if (!tmp)
                        exit(1); /* allocation failure */
                    cache = tmp;
                }
                cache[cache_cnt++] = next_candidate;
            }
        }

        /* advance to next candidate: skip even numbers */
        next_candidate += 2;
        if (next_candidate < 5)  /* handle wrap‑around for small start */
            next_candidate = 5;
    }

    return (seq_t)cache[(size_t)n - 1];
}
