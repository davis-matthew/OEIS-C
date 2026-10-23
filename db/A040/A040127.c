/*
 * A040127  Primes p such that x^4 = 13 has no solution mod p.
 * https://oeis.org/A040127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040127: primes p such that the congruence
   x^4 ≡ 13 (mod p) has no solution.  The routine tests each prime
   with modular exponentiation via mod_pow.  Results are cached
   in a static array, so subsequent calls with arbitrary n are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------------ */
/* Cache of computed sequence values.  cache[i] holds the (i+1)‑st
   prime with the specified property.  ------------------------------------------------------------------ */
static seq_t *cache       = NULL;   /* array of sequence values           */
static size_t cache_len   = 0;      /* number of terms stored             */
static size_t cache_cap   = 0;      /* allocated capacity of cache        */

/* ------------------------------------------------------------------ */
/* List of primes used for primality tests.  prime_list[i] is the
   (i+1)-st prime.  ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of primes found so far      */
static size_t    prime_len   = 0;      /* number of primes stored           */
static size_t    prime_cap   = 0;      /* allocated capacity of prime_list  */

/* ------------------------------------------------------------------ */
/* Ensure that the cache can hold at least `need` elements.  ------------------------------------------------------------------ */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that the prime list can hold at least `need` elements.  ------------------------------------------------------------------ */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Returns true if the congruence x^4 ≡ 13 (mod p) has a solution.  */
static bool has_fourth_root(uint64_t p)
{
    if (p == 13) return true;          /* 0 has a fourth root modulo 13 */

    seq_t a = (seq_t)(13 % p);          /* value to test                */

    uint64_t exp;
    if ((p & 3ULL) == 1)               /* p ≡ 1 (mod 4)                */
        exp = (p - 1) / 4;
    else                               /* p ≡ 3 (mod 4)                */
        exp = (p - 1) / 2;             /* square test suffices        */

    seq_t one = (seq_t)1;
    seq_t r = mod_pow(a, exp, (seq_t)p);
    return r == one;
}

/* ------------------------------------------------------------------ */
/* Generate primes and fill the cache until `target` terms are ready.  */
static void generate_until(idx_t target)
{
    /* Make sure we have at least one prime (2). */
    if (prime_len == 0) {
        ensure_prime_cap(1);
        prime_list[0] = 2;
        prime_len = 1;
    }

    /* Candidate for the next prime; start with the smallest odd number. */
    uint64_t candidate = (prime_list[prime_len - 1] <= 2) ? 3
                                                         : prime_list[prime_len - 1] + 2;

    while (cache_len < target) {
        /* Find the next prime. */
        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t pr = prime_list[i];
                if (pr > candidate / pr) break;       /* prevents overflow           */
                if (candidate % pr == 0) { isprime = false; break; }
            }
            if (isprime) break;
            candidate += 2;                            /* skip even numbers          */
        }

        /* Store the prime. */
        ensure_prime_cap(prime_len + 1);
        prime_list[prime_len++] = candidate;

        /* Test whether it belongs to the sequence. */
        if (!has_fourth_root(candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }

        /* Advance to the next odd candidate. */
        candidate += 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.  ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0     */
    generate_until(n);
    return cache[n - 1];
}
