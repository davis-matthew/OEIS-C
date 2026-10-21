/*
 * A061673  Even numbers k such that k+1 and k-1 are both composite.
 * https://oeis.org/A061673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning successive even integers and selecting those
   for which both neighboring odd integers are composite. */
/* Implementation uses a deterministic Miller–Rabin test for
   values that fit in 64‑bit unsigned integers.  The function keeps
   a static cache so that any order of calls is supported, and
   works for seq_t = uint8_t, uint16_t, uint32_t, uint64_t, or an
   arbitrary‑precision integer type (C++17). */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* Multiply two 64‑bit values modulo mod, using 128‑bit arithmetic. */

/* Exponentiation modulo mod, with 64‑bit inputs. */
static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1U)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

/* One Miller–Rabin witness test for 64‑bit integers. Returns
   true if a is a strong probable prime base. */
static bool witness(uint64_t n, uint64_t a)
{
    if (a % n == 0) return true;
    uint64_t d = n - 1, s = 0;
    while ((d & 1U) == 0U) { d >>= 1U; ++s; }
    uint64_t x = pow_mod64(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (uint64_t r = 1; r < s; ++r) {
        x = mul_mod64(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

/* Deterministic Miller–Rabin suited for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                 return false;
    /* Small primes: quick divisor test */
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* Deterministic bases for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        if (!witness(n, bases[i])) return false;
    }
    return true;
}

static bool is_composite(uint64_t v)
{
    return v > 1 && !is_prime_uint64(v);
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 : return 0 */

    /* Static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next even integer to check (carry between calls) */
    static uint64_t next_even = 2;

    while (cache_len < (size_t)n) {
        uint64_t k = next_even;

        /* Find the next even integer whose neighbors are composite */
        for (;;) {
            if (is_composite(k - 1) && is_composite(k + 1))
                break;
            k += 2U;
        }

        /* Grow the cache if needed */
        if (cache_len + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap << 1 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)k;   /* store the term */
        next_even = k + 2U;              /* next candidate starts after k */
    }
    return cache[n - 1];  /* 1‑based indexing */
}
