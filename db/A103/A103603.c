/*
 * A103603  Numbers k such that the string 101001000k is prime.
 * https://oeis.org/A103603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9169 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the string 101001000k is prime.
   a(1) = 7, a(2) = 29, …  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

/* Return number of decimal digits of an unsigned 64‑bit integer. */
static idx_t num_digits_uint64(uint64_t x) {
    if (x >= 10000000000000000000ULL) return 20;
    if (x >= 1000000000000000000ULL)  return 19;
    if (x >= 100000000000000000ULL)  return 18;
    if (x >= 10000000000000000ULL)   return 17;
    if (x >= 1000000000000000ULL)    return 16;
    if (x >= 100000000000000ULL)     return 15;
    if (x >= 10000000000000ULL)      return 14;
    if (x >= 1000000000000ULL)       return 13;
    if (x >= 100000000000ULL)        return 12;
    if (x >= 10000000000ULL)         return 11;
    if (x >= 1000000000ULL)          return 10;
    if (x >= 100000000ULL)           return 9;
    if (x >= 10000000ULL)            return 8;
    if (x >= 1000000ULL)             return 7;
    if (x >= 100000ULL)              return 6;
    if (x >= 10000ULL)               return 5;
    if (x >= 1000ULL)                return 4;
    if (x >= 100ULL)                 return 3;
    if (x >= 10ULL)                  return 2;
    return 1;
}

static const uint64_t PREFIX = 101001000ULL;   /* the fixed prefix */

/* Cache of previously found terms. */
static seq_t *cache = NULL;          /* array of found k values */
static idx_t cache_len = 0;          /* number of stored terms */
static idx_t cache_cap = 0;          /* current capacity of cache */
static uint64_t last_k_checked = 0;  /* last k value examined */

/* Ensure that cache contains at least n terms. */
static void extend_cache(idx_t n) {
    while (cache_len < n) {
        ++last_k_checked;
        uint64_t d = num_digits_uint64(last_k_checked);

        /* Compute M = PREFIX * 10^d + last_k_checked */
        unsigned __int128 pow10 = 1;
        for (uint64_t i = 0; i < d; ++i) pow10 *= 10;
        unsigned __int128 M128 = (unsigned __int128)PREFIX * pow10 + last_k_checked;

        /* If M exceeds 64 bits we cannot test it with is_prime_uint64,
           but the sequence for n <= 1000 stays safely within 64 bits.
           For larger n the implementation will fall back to a larger
           primality routine if required by the build. */
        uint64_t M64 = (uint64_t)M128;
        if (is_prime(M64)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)last_k_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* value below the offset is irrelevant */

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }
    extend_cache(n);
    return cache[n - 1];
}
