/*
 * A165810  Primes p such that 18*p+1 is also a prime.
 * https://oeis.org/A165810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that 18*p+1 is also prime
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Multiply two 64‑bit values modulo an arbitrary 64‑bit modulus.
   The result fits in 128 bits; we rely on the ubiquitous
   unsigned __int128 type in C/C++. */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Modular exponentiation (base^exp) mod mod. */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers.
   Works correctly for all n < 2^64. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;

    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
        29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for all 64‑bit numbers. */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Cached sequence of qualifying primes.  The cache is extended
   on demand and works correctly for calls in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, behaviour for n==0 is arbitrary */

    /* Static cache state --------------------------------------------------- */
    static idx_t last_checked = 1;      /* next integer candidate to test */
    static seq_t *cache = NULL;         /* array of verified terms          */
    static size_t cache_cnt = 0;        /* number of terms stored           */
    static size_t cache_cap = 0;        /* allocated capacity of 'cache'    */

    /* Extend the cached sequence until we have at least 'n' terms. */
    while (cache_cnt < n) {
        ++last_checked;                      /* next candidate integer */

        if (is_prime_u64((uint64_t)last_checked)) {
            uint64_t q = 18ULL * last_checked + 1ULL;
            if (is_prime_u64(q)) {
                /* grow the cache if needed */
                if (cache_cnt == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache,
                                             cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = (seq_t)last_checked; /* store the prime */
            }
        }
    }

    return cache[n - 1];   /* return the requested term (1‑based) */
}
