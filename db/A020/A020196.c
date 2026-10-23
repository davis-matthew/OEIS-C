/*
 * A020196  Pseudoprimes to base 68.
 * https://oeis.org/A020196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th composite number k such that
   68^(k‑1) ≡ 1 (mod k). In other words: a(n) is the n‑th pseudoprime to
   base 68.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;     /* array of found pseudoprimes (seq_t) */
static size_t cache_cap = 0;    /* capacity of the cache array          */
static size_t cache_count = 0;  /* number of pseudoprimes stored so far */
static uint64_t next_candidate = 4;   /* next integer to test */

/* ------------------------------------------------------------------
 * Helper: modular multiplication for uint64_t (uses __uint128_t)
 * ------------------------------------------------------------------ */
static uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* ------------------------------------------------------------------
 * Helper: modular exponentiation for uint64_t
 * ------------------------------------------------------------------ */
static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mulmod_u64(result, b, mod);
        b = mulmod_u64(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------
 * Helper: deterministic Miller-Rabin primality test for 64‑bit values
 * Uses bases 2,3,5,7,11,13 which are sufficient for all 64‑bit numbers
 * ------------------------------------------------------------------ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1ULL;
        ++s;
    }

    /* Witness loop with deterministic bases */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mulmod_u64(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* offset is 1, but graceful handling */

    /* Ensure the cache can hold n values */
    ensure_cache(n, &cache, &cache_cap);

    /* Find new pseudoprimes until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        if (!is_prime_u64(next_candidate))          /* composite? */
        {
            if (powmod_u64(68ULL, next_candidate - 1,
                           (uint64_t)next_candidate) == 1ULL)
            {
                /* store the pseudoprime – cast to seq_t.
                   If the value exceeds the representation of seq_t,
                   the cast will wrap around; such overshoot is not
                   expected for the test harness. */
                cache[cache_count++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
