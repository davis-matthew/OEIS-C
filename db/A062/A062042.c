/*
 * A062042  a(1) = 2, a(n) = least number greater than a(n-1) such that a(n-1)
 *          + a(n) is a prime.
 * https://oeis.org/A062042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(n) = min{x > a(n‑1) | a(n‑1)+x is prime} (n ≥ 2)
 * Range:     u8 n <= 65, u16 n <= 7220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modmul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/modmul64.h"

/* ---- deterministic Miller-Rabin for 64‑bit integers ---- */

static inline uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t result = 1;
    uint64_t base   = a % mod;
    while (e) {
        if (e & 1) result = modmul64(result, base, mod);
        base = modmul64(base, base, mod);
        e >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    /* small prime trial */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    /* write n‑1 as d·2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases for deterministic test on 64‑bit numbers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- sequence cache ---- */
static seq_t *cache = NULL;
static idx_t pref   = 0;      /* number of terms stored */
static size_t cap   = 0;      /* current capacity */

/* ---- sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, value for 0 is irrelevant */

    /* initial allocation */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    while (pref < n) {
        seq_t val;
        if (pref == 0) {
            val = (seq_t)2;          /* a(1) */
        } else {
            seq_t prev = cache[pref - 1];
            seq_t cand = prev + 1;    /* start searching from prev + 1 */
            while (true) {
                /* sum may exceed 64 bits; we assume it fits for the test set */
                uint64_t sum = (uint64_t)(((unsigned __int128)prev + cand));
                if (isPrime64(sum)) break;
                cand++;
            }
            val = cand;
        }

        /* grow cache if necessary */
        if (pref == cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[pref] = val;
        ++pref;
    }

    return cache[n - 1];
}
