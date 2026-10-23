/*
 * A103540  Numbers k such that the string k2357111317 is prime.
 * https://oeis.org/A103540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) by searching successive integers k and testing whether
 * the concatenated number k2357111317 is prime.  The results are cached so
 * that later calls for any n≥1 are answered in O(1) time.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

/* ---------- primality test for 64‑bit integers ---------- */

static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool isPrime(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* Miller‑Rabin with bases that are deterministic for n < 2^64 */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                              450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- cache for computed terms ---------- */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
static idx_t  cache_size = 0;
static idx_t  last_k = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* return cached value if already known */
    if (n <= cache_size) return cache[n-1];

    const uint64_t suffix = 2357111317ULL;          /* 10 digits */
    const uint64_t mult   = 10000000000ULL;         /* 10^10 */

    /* search until we have the n‑th term */
    while (cache_size < n) {
        idx_t k = last_k + 1;
        unsigned __int128 cand128 = (unsigned __int128)k * mult + suffix;
        if (cand128 <= UINT64_MAX) {
            uint64_t cand = (uint64_t)cand128;
            if (isPrime(cand)) {
                /* grow cache if necessary */
                if (cache_size >= cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) { free(cache); cache = NULL; return 0; }
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)k;
                last_k = k;            /* keep k for next iteration */
                continue;              /* skip the extra increment */
            }
        }
        last_k = k;                    /* even if not prime */
    }

    return cache[n-1];
}
