/*
 * A094516  Primes p such that q=(p^2+1)/2 is not a prime.
 * https://oeis.org/A094516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5772, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime p such that (p^2+1)/2 is NOT a prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul.h"

/* ------------------------------------------------------------ */
/* Miller–Rabin primality test for 64‑bit unsigned integers.    */
/* ------------------------------------------------------------ */

static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1)
            result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* small primes */
    const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0)
            return n == small[i];
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for all 64‑bit integers */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 2;   /* last candidate examined */
    static bool   initialized   = false;

    /* One‑time initialization */
    if (!initialized) {
        initialized = true;
        cache_cap  = 8;
        cache_len  = 1;               /* start with prime 2 */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
    }

    /* Extend cache until at least n qualifying primes are stored */
    while (cache_len < (size_t)n) {
        /* find next prime candidate */
        ++last_checked;
        if (last_checked > 2 && ((last_checked & 1u) == 0))
            continue;                    /* skip even numbers > 2 */

        if (!is_prime_uint64((uint64_t)last_checked))
            continue;

        uint64_t p = (uint64_t)last_checked;
        uint64_t q = (p * p + 1ULL) / 2ULL;

        if (!is_prime_uint64(q)) {
            /* store qualifying prime */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
