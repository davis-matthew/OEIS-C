/*
 * A051099  Primes p such that x^60 = -2 has a solution mod p.
 * https://oeis.org/A051099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a prime p such that x^60 ≡ -2 (mod p). For p > 2 this is
 *            equivalent to (-2 mod p) being a 60th power residue modulo p:
 *            let d = gcd(60, p‑1), then the condition is ((p‑2)^((p‑1)/d) mod
 *            p == 1). The prime 2 is included by definition. The function
 *            searches primes in order, caches the ones satisfying...
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_u64, _mod_pow, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>      /* for abort() */

#include "../utils/isPrime.h"      /* bool isPrime(idx_t) */
#include "../utils/_gcd_u64.h"    /* uint64_t _gcd_u64(uint64_t, uint64_t) */
#include "../utils/_mod_pow.h"    /* seq_t _mod_pow(seq_t, uint64_t, seq_t) */

static seq_t *prime_cache = NULL;   /* cached primes satisfying the condition */
static size_t  cache_len   = 0;
static size_t  cache_cap   = 0;
static uint64_t next_candidate = 2;  /* smallest prime to test next */

/* Grow the cache array to at least 'needed' entries. */
static void cache_grow(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* Append a prime satisfying the condition to the cache. */
static void cache_append(seq_t val)
{
    cache_grow(cache_len + 1);
    prime_cache[cache_len++] = val;
}

/* Return the next prime >= current candidate, updating the candidate for
   subsequent calls.  The helper isPrime uses idx_t (uint64_t). */
static uint64_t next_prime(void)
{
    while (!isPrime((idx_t)next_candidate))
        ++next_candidate;
    uint64_t p = next_candidate;
    ++next_candidate;
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless value */

    while (cache_len < (size_t)n) {
        uint64_t p = next_prime();
        if (p == 2) {
            cache_append((seq_t)2);        /* 2 is always in the sequence */
            continue;
        }
        uint64_t a_val = p - 2;                 /* equivalent to (-2 mod p) */
        uint64_t d     = _gcd_u64(60ULL, p - 1);/* gcd(60, p-1) */
        uint64_t exp   = (p - 1) / d;           /* exponent */
        seq_t res = _mod_pow((seq_t)a_val, exp, (seq_t)p);
        if (res == (seq_t)1)                    /* power residue condition */
            cache_append((seq_t)p);
    }
    return prime_cache[n - 1];
}
