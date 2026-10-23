/*
 * A108854  Numbers k such that 10*k - 127 is prime.
 * https://oeis.org/A108854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A108854 lists all integers k such that 10*k - 127 is prime. 
   We generate the terms by scanning k > 12, testing 10*k-127 for primality. 
   A deterministic Miller–Rabin test for 64‑bit integers is implemented locally. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    __uint128_t res = ( __uint128_t )a * b % mod;
    return (uint64_t)res;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;

    /* lazy allocation */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    if ((size_t)n > sz) {
        /* start searching from the next k after the last found */
        uint64_t current_k = (sz == 0) ? 1U : (uint64_t)cache[sz-1] + 1U;

        while (sz < (size_t)n) {
            uint64_t p = 10ULL * current_k - 127ULL;
            if (is_prime_uint64(p)) {
                /* ensure capacity */
                if (sz == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[sz] = (seq_t)current_k;
                ++sz;
            }
            ++current_k;
        }
    }

    return cache[n-1];
}
