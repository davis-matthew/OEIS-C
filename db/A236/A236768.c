/*
 * A236768  A recurrence relation conditioned on the primality of the
 *          preceding terms.
 * https://oeis.org/A236768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = a(n-1)+a(n-2) if at least one of
 *            a(n-1), a(n-2) is prime, otherwise a(n) = a(n-1)-a(n-2).
 *            Sequence A236768
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 138, u64 n <= 366,
 *            u128 n <= 418, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* If seq_t may be >64 bits, we fall back to the provided big-prime test.
   The helper is provided by the common utils directory. */
#include "../utils/is_prime_big.h"

/* ---------- Deterministic Miller‑Rabin for 64‑bit integers ---------- */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (__uint128_t)a * b % m;
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t a = small_primes[i];
        if (a >= n) break;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool passed = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}

/* Generic primality test: use the fast 64‑bit routine when possible,
   otherwise defer to the big‑int helper. */
static bool generic_is_prime(seq_t n)
{
    /* Check if n fits in uint64_t */
    if (sizeof(seq_t) <= 8) {
        /* For seq_t 8 bytes, the cast is safe for all values */
        return is_prime_uint64((uint64_t)n);
    }
    /* For larger seq_t types, use the provided big‑int primality test. */
    return is_prime_big(n);
}

/* ---------- Memoized sequence implementation ---------- */
static seq_t *cache = NULL;   /* memoized values of a(n) */
static size_t cap  = 0;       /* allocated capacity for cache array */
static idx_t len    = 0;      /* number of values currently stored */

/* Ensure the cache can hold entries up to index idx (inclusive). */
static void ensure_cache(idx_t idx)
{
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while ((size_t)(idx + 1) > cap) {
        cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }
}

/* Return a(n) for any non‑negative n; memoized for efficiency. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    ensure_cache(n);

    /* Initialize base cases if cache is empty. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        len = 2;
    }

    /* Extend cache up to n. */
    for (idx_t i = len; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        bool p1 = generic_is_prime(prev1);
        bool p2 = generic_is_prime(prev2);

        cache[i] = (p1 || p2) ? prev1 + prev2 : prev1 - prev2;
    }
    len = n + 1;
    return cache[n];
}
