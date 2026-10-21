/*
 * A089238  Numbers k such that 3*k^2/2 - 1 is a prime.
 * https://oeis.org/A089238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089238: Numbers k such that 3*k^2/2 - 1 is prime.
   k is always even, write k = 2*m. Then the condition becomes
   6*m^2 - 1 is prime.  a(n) returns the n‑th such k  (offset 1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;

        __uint128_t x = 1, base = a, exp = d;
        while (exp > 0) {
            if (exp & 1ULL)
                x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1)
            continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            xx = (__uint128_t)xx * xx % n;
            if (xx == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Dynamic array for cached terms. */
static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static idx_t lastM = 0;

/* Ensure sufficient capacity for the term cache. */
static void ensureCacheCapacity(size_t needed)
{
    if (needed <= cacheCap)
        return;
    size_t newCap = cacheCap ? cacheCap * 2 : 16;
    while (newCap < needed)
        newCap *= 2;
    seq_t *newPtr = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (!newPtr)
        exit(1); /* allocation failure */
    cache = newPtr;
    cacheCap = newCap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for offset 1, safe fallback */
    while (cacheSize < n) {
        ++lastM;
        /* candidate = 6*m^2 - 1, compute using 128‑bit to avoid overflow */
        __uint128_t prod = (__uint128_t)6 * lastM * lastM;
        uint64_t cand = (uint64_t)(prod - 1);
        if (prod - 1 > UINT64_MAX)          /* safety check – we only test up to 64‑bit */
            break;
        if (isPrime64(cand)) {
            ensureCacheCapacity(cacheSize + 1);
            cache[cacheSize++] = (seq_t)(2ULL * lastM); /* k = 2*m */
        }
    }
    if (n > cacheSize)
        return (seq_t)0;                 /* shouldn't happen for valid inputs within range */
    return cache[n - 1];
}
