/*
 * A090833  Numbers n such that 6n+5, 6n+11, 6n+17, 6n+23 are consecutive
 *          primes or 6n+1, 6n+7, 6n+13, 6n+19 are consecutive primes.
 * https://oeis.org/A090833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090833: n such that 6n+5, 6n+11, 6n+17, 6n+23 or
   6n+1, 6n+7, 6n+13, 6n+19 are four consecutive primes.
   The function computes the n‑th term by brute‑force search using a
   deterministic Miller–Rabin test for 64‑bit integers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/modmul.h"

/* ---------- Miller–Rabin for 64‑bit integers ---------- */

static inline uint64_t modpow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e) {
        if (e & 1u)
            r = modmul(r, a, mod);
        a = modmul(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;

    /* trial division by small primes */
    static const uint32_t small[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < 12; ++i) {
        uint32_t p = small[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1u) == 0u) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for 64‑bit integers */
    static const uint64_t bases[7] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0u)    /* base divides n */
            continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul(x, x, n);
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp)
            return false;
    }
    return true;
}

/* ---------- pattern checks ---------- */

static bool patternA(uint64_t n)
{
    /* primes at 6n+5, 6n+11, 6n+17, 6n+23 */
    uint64_t p = 6 * n + 5;
    if (!isPrime64(p))          return false;
    if (!isPrime64(p + 6))      return false;
    if (!isPrime64(p + 12))     return false;
    if (!isPrime64(p + 18))     return false;

    /* composites at 6n-1, 6n+7, 6n+13, 6n+19, 6n+29 */
    if (isPrime64(p - 6))       return false;    /* 6n-1 */
    if (isPrime64(p + 2))       return false;    /* 6n+7 (p+2) */
    if (isPrime64(p + 8))       return false;    /* 6n+13 (p+8) */
    if (isPrime64(p + 14))      return false;    /* 6n+19 (p+14) */
    if (isPrime64(p + 24))      return false;    /* 6n+29 (p+24) */
    return true;
}

static bool patternB(uint64_t n)
{
    /* primes at 6n+1, 6n+7, 6n+13, 6n+19 */
    uint64_t p = 6 * n + 1;
    if (!isPrime64(p))          return false;
    if (!isPrime64(p + 6))      return false;
    if (!isPrime64(p + 12))     return false;
    if (!isPrime64(p + 18))     return false;

    /* composites at 6n-5, 6n+5, 6n+11, 6n+17, 6n+25 */
    if (isPrime64(p - 6))       return false;    /* 6n-5 (p-6) */
    if (isPrime64(p + 4))       return false;    /* 6n+5 (p+4) */
    if (isPrime64(p + 10))      return false;    /* 6n+11 (p+10) */
    if (isPrime64(p + 16))      return false;    /* 6n+17 (p+16) */
    if (isPrime64(p + 24))      return false;    /* 6n+25 (p+24) */
    return true;
}

/* ---------- memoised sequence generator ---------- */

static seq_t *cache = NULL;
static idx_t cache_len = 0;          /* number of stored terms */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t last_checked = 0;       /* highest n examined so far */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1 */
        return 0;

    if (n <= cache_len)             /* already known */
        return cache[n - 1];

    /* grow cache to hold at least n elements */
    while (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            cache_len = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    uint64_t cand = (uint64_t)last_checked + 1;
    while (cache_len < n) {
        if (cand % 5 == 0 || cand % 5 == 1) {   /* skip obvious composites */
            if (patternA(cand) || patternB(cand))
                cache[cache_len++] = (seq_t)cand;
        }
        ++cand;
    }

    last_checked = (idx_t)(cand - 1);
    return cache[n - 1];
}
