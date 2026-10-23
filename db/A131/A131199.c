/*
 * A131199  a(0)=2. For n>=1, a(n) = a(n-1) + n if a(n-1) is composite; a(n) =
 *          a(n-1)*n if a(n-1) is prime.
 * https://oeis.org/A131199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 88, u64 n <= 274,
 *            u128 n <= 398, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=2.
   For n>=1, if a(n-1) is composite then a(n)=a(n-1)+n;
   if a(n-1) is prime then a(n)=a(n-1)*n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Deterministic Miller–Rabin for 64‑bit integers. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes first */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0ULL) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;

        __uint128_t x = 1, base = a, exp = d, mod = n;
        while (exp) {
            if (exp & 1ULL) x = (x * base) % mod;
            base = ( (__uint128_t)base * base ) % mod;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Dispatch to the appropriate primality test. */
static inline bool is_prime_seq(seq_t n)
{
    /* If seq_t fits in uint64_t, use the fast test above. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_uint64((uint64_t)n);
    /* For larger types, fall back to the provided generic test. */
    return is_prime_big(n);
}

/* Ensure capacity for cache. */
static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = needed * 2 + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* Compute terms up to n. */
    ensure_cache_size(n + 1);

    if (cache_size == 0) {
        cache[0] = (seq_t)2;
        cache_size = 1;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        bool prime = is_prime_seq(prev);
        seq_t v;
        if (prime)
            v = prev * (seq_t)i;
        else
            v = prev + (seq_t)i;
        cache[i] = v;
    }

    cache_size = n + 1;
    return cache[n];
}
