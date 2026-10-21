/*
 * A086478  Primes p such that 512p+1 and (p-1)/512 are both prime.
 * https://oeis.org/A086478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterates over primes of the form p = 512k+1 and
   checks that both k and 512p+1 are prime. The sequence values
   are cached so that any call order is efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64.h"

/* 64‑bit modular multiplication using 128‑bit arithmetic */

/* 64‑bit modular exponentiation */
static inline uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul64(r, a, mod);
        a = mul64(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cache of computed terms */
static seq_t *cache = NULL;          /* array of found primes p */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of the array */
static uint64_t last_k = 0;          /* last k value processed (p = 512*k+1) */

/* Ensure that the cache has capacity for at least 'need' elements */
static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
#if defined(__cplusplus)
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#else
    seq_t *new_ptr = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!new_ptr) exit(1);          /* Out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for n<1, return 0 as guard */

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Generate next candidate prime p = 512*k+1 */
        ++last_k;
        uint64_t k = last_k;
        uint64_t p = 512ULL * k + 1ULL;
        if (!isPrime64(p)) continue;  /* p must be prime */

        if (!isPrime64(k)) continue;  /* k must be prime */

        uint64_t q = 512ULL * p + 1ULL;   /* 512p+1 must be prime */
        if (!isPrime64(q)) continue;

        /* Store p in cache */
        ensure_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)p;
    }

    /* Return the (n-1)-th cached term as seq_t */
    return cache[n - 1];
}
