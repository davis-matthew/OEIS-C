/*
 * A136242  Numbers k among A008864 such that k^2 - k + 1 is prime.
 * https://oeis.org/A136242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that k-1 is prime and k^2 - k + 1
 *            is prime
 * Range:     u8 n <= 12, u16 n <= 503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit modular multiplication avoiding overflow */
static inline uint64_t modmul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation */
static inline uint64_t modpow_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = modmul_u64(r, a, mod);
        a = modmul_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 = d * 2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* witnesses sufficient for 64‑bit integers */
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul_u64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; callers should use n>=1 */

    static seq_t *cache = NULL;     /* sequence values */
    static idx_t cache_used = 0;    /* number of values cached */
    static idx_t cache_cap   = 0;    /* allocated capacity */
    static idx_t last_k = 2;        /* last k examined (initially 2) */

    while (cache_used < n) {
        /* grow cache if needed */
        if (cache_cap == 0) {
            cache_cap = 256;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_used == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* find next k such that k-1 is prime and k^2-k+1 is prime */
        do {
            ++last_k;
        } while (!is_prime_u64(last_k - 1) ||
                 !is_prime_u64((uint64_t)last_k * last_k - last_k + 1));

        /* store the qualifying k */
        cache[cache_used++] = (seq_t)last_k;
    }

    return cache[n - 1];
}
