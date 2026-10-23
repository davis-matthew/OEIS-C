/*
 * A040138  Primes p such that x^4 = 17 has a solution mod p.
 * https://oeis.org/A040138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A040138 – primes p such that x^4 ≡ 17 (mod p) has a solution */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_sqrt.h"            /* int_sqrt(idx_t) */

static seq_t *cache    = NULL;          /* cached sequence terms          */
static size_t cache_len = 0;            /* number of cached terms          */
static size_t cache_cap = 0;            /* allocated cache capacity        */

static uint64_t *prime_list = NULL;     /* found primes so far             */
static size_t prime_len = 0;            /* number of primes in list        */
static size_t prime_cap = 0;            /* allocated prime list capacity   */

static uint64_t next_candidate = 2;     /* next integer to test for prime */

/* Ensure cache has capacity for at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Ensure prime list has capacity for at least 'need' elements. */
static void ensure_prime_capacity(size_t need)
{
    if (need > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
}

/* 128‑bit multiplication modulo: prevent overflow. */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Modular exponentiation: base^exp mod mod. */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U) result = mul_mod64(result, b, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

/* Simple primality test using previously found primes. */
static bool is_prime_uint64(uint64_t p)
{
    if (p < 2) return false;
    if (p == 2) return true;
    if ((p & 1ULL) == 0) return false;
    uint64_t lim = int_sqrt(p);
    for (size_t i = 0; i < prime_len && prime_list[i] <= lim; ++i)
        if (p % prime_list[i] == 0) return false;
    return true;
}

/* Test whether 17 is a fourth‑power residue modulo odd prime p. */
static bool fourth_power_residue(uint64_t p)
{
    if (p == 2) return true;          /* 17 ≡ 1 (mod 2) */
    if (p == 17) return true;         /* 17 ≡ 0 (mod 17), solvable by x = 0 */
    uint64_t a = 17 % p;
    uint64_t g = ((p - 1) % 4 == 0) ? 4 : 2;
    uint64_t exp = (p - 1) / g;
    return mod_pow64(a, exp, p) == 1;
}

/* --------------------------------------------------------------------- */
/*  Return the nth term of the sequence.                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1 */

    /* Generate primes and test the property until we have n terms. */
    while (cache_len < n) {
        /* Find next prime candidate. */
        while (1) {
            uint64_t p = next_candidate;
            if (is_prime_uint64(p)) {
                /* Store the new prime. */
                ensure_prime_capacity(prime_len + 1);
                prime_list[prime_len++] = p;

                /* Test the quartic residue condition. */
                if (fourth_power_residue(p)) {
                    ensure_cache_capacity(cache_len + 1);
                    cache[cache_len++] = (seq_t)p;
                }
                /* Next candidate is the following integer. */
                next_candidate = p + 1;
                break;
            } else {
                ++next_candidate;
            }
        }
    }
    return cache[n - 1];
}
