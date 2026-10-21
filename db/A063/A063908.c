/*
 * A063908  Numbers k such that k and 2*k-3 are primes.
 * https://oeis.org/A063908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the n-th prime k such that (2*k-3) is also prime.
   The implementation lazily searches odd numbers, tests both the
   number and its partner with a deterministic Miller‑Rabin routine
   (sufficient for 64‑bit values), and caches found terms for
   subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/*----- deterministic Miller‑Rabin for 64‑bit integers ---------*/

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) return false;
    }
    return true;
}

static bool _is_prime(uint64_t n) {
    if (n < 2) return false;
    /* small primes, fast path */
    static const uint64_t smalls[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(smalls)/sizeof(smalls[0]); ++i) {
        if (n == smalls[i]) return true;
        if (n % smalls[i] == 0) return false;
    }
    /* Write n-1 as d * 2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* deterministic bases for 64-bit */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool witness = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/*--------------------------------------------------------------*/

seq_t a(idx_t n) {
    /* static cache of successive terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next odd candidate to test (starting at 3) */
    static idx_t candidate = 3;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n terms */
    while (cache_len < n) {
        if (_is_prime((uint64_t)candidate)) {
            unsigned __int128 tmp = (unsigned __int128)candidate * 2 - 3;
            uint64_t partner = (uint64_t)tmp;
            if (_is_prime(partner)) {
                if (cache_len >= cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        candidate += 2;
    }

    return cache[n - 1];
}
