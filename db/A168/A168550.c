/*
 * A168550  Natural numbers k for which 1 + 2*k^3 is prime.
 * https://oeis.org/A168550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th natural number k such that 1 + 2*k^3 is prime
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof smallPrimes / sizeof smallPrimes[0]; ++i) {
        if (n == smallPrimes[i]) return true;
        if (n % smallPrimes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Witnesses sufficient for all 64‑bit integers. */
    static const uint64_t witnesses[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof witnesses / sizeof witnesses[0]; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        /* modular exponentiation */
        __uint128_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache of found k values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t current_k = 1;

/* Ensure the cache has enough capacity. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr) { /* allocation failure, abort */
        abort();
    }
    cache = newptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard */
    /* Fill cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        /* Compute 1 + 2*k^3 using 128‑bit arithmetic to avoid overflow. */
        __uint128_t k128 = current_k;
        __uint128_t k3 = k128 * k128 * k128;          /* k^3 */
        __uint128_t val128 = 1 + (__uint128_t)2 * k3; /* 1 + 2*k^3 */
        /* val should fit in 64 bits for n up to about 10 000 */
        uint64_t val = (uint64_t)val128;
        if (isPrime64(val)) {
            /* Cache the k value. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)current_k;
        }
        ++current_k;
    }
    return cache[n - 1];
}
