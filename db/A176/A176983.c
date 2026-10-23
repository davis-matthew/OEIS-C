/*
 * A176983  Primes p such that smallest prime q > p^2 is of form q = p^2 +
 *          k^2.
 * https://oeis.org/A176983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mulmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A176983 – Primes p such that the smallest prime q > p² is of the
    form q = p² + k².
    The code searches primes in increasing order, checks the condition,
    remembers all qualifying primes in a dynamic cache, and returns the
    n‑th element (1‑based indexing).  The algorithm is O(number of primes searched).  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mulmod_u64.h"

/* 128‑bit modular multiplication (no overflow for 64‑bit n) */

/* binary exponentiation modulo mod */
static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mulmod_u64(result, base, mod);
        base = mulmod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for n < 2⁶⁴ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n % small[i] == 0)
            return n == small[i];
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
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

/* next prime strictly greater than start (start may be 0 or 1) */
static uint64_t next_prime_u64(uint64_t start)
{
    uint64_t cand = start + 1;
    if (cand <= 2)
        return 2;
    if (cand % 2 == 0)
        ++cand;
    for (;;) {
        if (is_prime_u64(cand))
            return cand;
        cand += 2;
    }
}

/* integer square root of unsigned 64‑bit number (floor) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0)
        return 0;
    uint64_t x = n;
    uint64_t y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ----------  Sequence implementation  ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
/* start value for search of next prime p (strictly greater than this) */
static uint64_t prime_start = 1;

/* enlarge cache to at least min_cap elements */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(*new_ptr));
    if (!new_ptr && new_cap != 0)
        abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for 0, but safe return */

    while (cache_len < (size_t)n) {
        /* find next prime p */
        uint64_t p = next_prime_u64(prime_start);
        prime_start = p;          /* next search starts after this prime */

        /* p² */
        unsigned __int128 p2_big = (unsigned __int128)p * p;
        uint64_t p2 = (uint64_t)p2_big;   /* fits in 64 bits for our range */

        /* smallest prime q > p² */
        uint64_t q = next_prime_u64(p2);

        /* check if q − p² is a perfect square */
        uint64_t diff = q - p2;
        uint64_t root = isqrt_u64(diff);
        if ((unsigned __int128)root * root == diff) {
            /* store p */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1];
}
