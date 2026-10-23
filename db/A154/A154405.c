/*
 * A154405  Primes of the form 20n^2+8n+1.
 * https://oeis.org/A154405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2679, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime of the form 20*n*n + 8*n + 1.
   The function keeps an internal cache of already found primes
   so that calls can come in arbitrary order. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Deterministic Miller–Rabin for 64‑bit numbers.
   Works for all 64‑bit unsigned integers. */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((__uint128_t)a * b % m);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = mod_mul(result, b, mod);
        b = mod_mul(b, b, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    /* Write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
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

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Cache of found primes of the form 20*k^2+8*k+1. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;   /* 1‑based index for k */

    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t val = 20 * (k * k) + 8 * k + 1;

        /* Test primality using 64‑bit Miller–Rabin */
        if (is_prime_uint64((uint64_t)val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
