/*
 * A182091  Numbers n such that (n-1)*n^2/2-1 is prime.
 * https://oeis.org/A182091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number n such that ((n-1)*n^2)/2 - 1 is prime
 * Range:     u8 n <= 58, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_big.h"

/* ------------ deterministic Miller–Rabin for 64‑bit values ------------- */

static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1UL) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        uint64_t r;
        for (r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------- cache of found terms --------------------------- */

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;
static idx_t next_candidate = 2;

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_capacity) return;
    idx_t new_cap = cache_capacity == 0 ? 16 : cache_capacity * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = ptr;
    cache_capacity = new_cap;
}

/* ----------------------------- a ---------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but avoid UB   */
    while (cache_size < n) {
        /* compute ((cand-1)*cand^2)/2 - 1  in 128‑bit to avoid overflow   */
        __uint128_t cand = next_candidate;
        __uint128_t val = ((cand - 1) * cand * cand) / 2 - 1;

        bool prime;
        if (val <= (__uint128_t)UINT64_MAX) {
            prime = is_prime64((uint64_t)val);
        } else {
            seq_t big_val = (seq_t)val;
            prime = is_prime_big(big_val);
        }

        if (prime) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
