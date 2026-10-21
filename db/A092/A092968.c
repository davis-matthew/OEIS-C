/*
 * A092968  Numbers n such that 2n^2 + 11 is a prime.
 * https://oeis.org/A092968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 2n^2+11 is prime. a(1)=0, a(2)=1, …
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ---------- 64‑bit deterministic Miller–Rabin primality test ---------- */

static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}
static bool is_prime_ull(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t smallPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        if (n % smallPrimes[i] == 0) return n == smallPrimes[i];
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------  a(idx_t n) implementation ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined offset – return 0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_candidate = 0;   /* integer whose 2n^2+11 is tested */
    static idx_t found = 0;              /* number of found terms */

    /* grow cache until it contains the requested index (n) */
    while (found < n) {
        uint64_t val = 2ULL * next_candidate * next_candidate + 11ULL;
        if (is_prime_ull(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;   /* implicit conversion */
            ++found;
        }
        ++next_candidate;
    }
    return cache[n-1];
}
