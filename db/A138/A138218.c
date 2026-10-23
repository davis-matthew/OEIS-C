/*
 * A138218  Numbers k such that 180k^2 + 1 is prime.
 * https://oeis.org/A138218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sqrt((A137530(n) - 1)/180)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Deterministic Miller–Rabin test for 64–bit integers.
 * Bases 2, 325, 9375, 28178, 450775, 9780504, 1795265022
 * guarantee correctness for all 64‑bit unsigned values. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division by small primes to speed up obvious composites */
    static const uint64_t sp[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(sp)/sizeof(sp[0]); ++i) {
        uint64_t p = sp[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        uint64_t x = 1, base = a, exp = d, mod = n;
        /* modular exponentiation */
        while (exp) {
            if (exp & 1) x = (unsigned __int128)x * base % mod;
            base = (unsigned __int128)base * base % mod;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool continue_loop = false;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % mod;
            if (x == n - 1) { continue_loop = true; break; }
        }
        if (!continue_loop) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 1;          /* 64‑bit counter for k */

    if (n == 0) return 0;                /* safety for out–of–range */

    while (cache_len < n) {
        uint64_t k          = next_k++;
        unsigned __int128 v = (unsigned __int128)180 * k * k + 1;
        uint64_t val32      = (uint64_t)v;   /* fits into 64‑bit for all n up to 10 000 */

        if (is_prime_u64(val32)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
