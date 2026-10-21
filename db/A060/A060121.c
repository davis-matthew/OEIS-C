/*
 * A060121  First solution mod p of x^3 = 2 for primes p such that only one
 *          solution exists.
 * https://oeis.org/A060121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A060121
    a(n) = first (only) solution to x³ = 2 modulo the n‑th prime
    for which x³ = 2 has exactly one solution (i.e., p = 2, 3 or p ≡ 2 (mod 3)).
    For p ≡ 2 (mod 3) the solution is x = 2^((2p–1)/3) (mod p) (since 3⁻¹ ≡ (2p–1)/3 mod (p–1)).
    For p = 2 the unique solution is 0, for p = 3 it is 2. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static state */
    static uint64_t *primes   = NULL;
    static size_t primes_cnt  = 0;
    static size_t primes_cap  = 0;
    static uint64_t *specials = NULL;
    static size_t specials_cnt = 0;
    static size_t specials_cap = 0;
    static uint64_t candidate = 3;   /* start with the first odd number to test */

    /* one‑time allocation */
    if (!primes) {
        primes_cap   = 64;
        specials_cap = 64;
        primes   = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        specials = (uint64_t *)malloc(specials_cap * sizeof(uint64_t));
        if (!primes || !specials) abort();

        /* initialise with the prime 2 – the only even special prime */
        primes[0] = 2;
        primes_cnt = 1;
        specials[0] = 2;
        specials_cnt = 1;
    }

    /* helper to grow the arrays when needed */
    #define GROW(arr, cap, cnt)            \
        if ((cnt) >= (cap)) {               \
            (cap) = (cap) ? (cap) * 2 : 64; \
            (arr) = (uint64_t *)realloc((arr), (cap) * sizeof(uint64_t)); \
            if (!(arr)) abort();                 \
        }

    /* generate primes until we have the n‑th special prime */
    while (specials_cnt < (size_t)n) {
        uint64_t x = candidate;

        /* trial‑division primality test using already found primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > x / p)  /* avoid overflow in p*p */
                break;
            if (x % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store the new prime */
            GROW(primes, primes_cap, primes_cnt);
            primes[primes_cnt++] = x;

            /* check if it is a special prime */
            if (x == 3 || x % 3 == 2) {
                GROW(specials, specials_cap, specials_cnt);
                specials[specials_cnt++] = x;
            }
        }

        candidate += 2;   /* only odd numbers can be prime (except 2) */
    }

    uint64_t p = specials[n - 1];

    /* compute the unique cubic root modulo p */
    if (p == 2) {
        return (seq_t)0;      /* 0³ ≡ 0 ≡ 2 (mod 2) */
    } else if (p == 3) {
        return (seq_t)2;      /* 2³ ≡ 8 ≡ 2 (mod 3) */
    } else {
        /* exponent (2p – 1) / 3 is an integer for p ≡ 2 (mod 3) */
        uint64_t exp = (2 * p - 1) / 3;
        return mod_pow((seq_t)2, exp, (seq_t)p);
    }
}
