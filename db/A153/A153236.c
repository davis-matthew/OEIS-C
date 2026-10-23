/*
 * A153236  Numbers n such that 8*n + 3 is not prime.
 * https://oeis.org/A153236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modmul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer k such that 8*k + 3 is composite.
 * This memoises the sequence using a lazy sieve-like approach.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/modmul.h"

/* ---------- Local primality test for 64‑bit integers ---------- */

static inline uint64_t modpow(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = modmul(r, a, mod);
        a = modmul(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_ull(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases for deterministic test over 64 bits */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        if (bases[i] % n == 0) return true;
        uint64_t x = modpow(bases[i], d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Sequence implementation ---------- */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t next_candidate = 1;  /* next integer to test */

    while (cache_size < n) {
        /* compute 8*k + 3 in 64‑bit to avoid overflow of seq_t */
        uint64_t test_val = (uint64_t)next_candidate * 8ULL + 3ULL;

        if (!is_prime_ull(test_val)) {
            if (cache_size == cache_cap) {        /* grow storage */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) {
                    /* allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;  /* store value */
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
