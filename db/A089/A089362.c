/*
 * A089362  Numbers k such that k^2 - 5k + 5 is prime.
 * https://oeis.org/A089362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer k such that k*k - 5*k + 5 is prime
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches k sequentially, tests the primality of k*k‑5*k+5, */
/* caches the found terms and supports calls in arbitrary order.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Deterministic Miller‑Rabin for 64‑bit values: bases 2,3,5,7,11,13,17. */
static inline uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod) {
    /* Use 128‑bit intermediate to avoid overflow (available in C90+) */
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool isPrimeUint64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* Miller‑Rabin */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* static cache for found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 1;          /* start from k = 1 */
    if (n == 0) return 0;               /* undefined, but guard against zero */

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) return cache[n-1];

    /* Grow cache as needed within loop */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k;                                 /* 64‑bit temporary */
        uint64_t val = k * k - 5 * k + 5;                    /* f(k) */
        if (isPrimeUint64(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure -> undefined behaviour */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;                   /* store k */
        }
        ++next_k;
    }
    return cache[n-1];
}
