/*
 * A051884  Smallest number larger than the previous term which is not a prime
 *          but is relatively prime to the previous term.
 * https://oeis.org/A051884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1)=1. For n>1, a(n) is the smallest integer k>a(n-1) such that
 *            k is not prime and gcd(k, a(n-1))=1
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Compute gcd for 64‑bit unsigned integers */

/* Simple deterministic primality test for 64‑bit unsigned integers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t step = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += step;
        step = 6 - step;
    }
    return true;
}

/* Sequence function */
seq_t a(idx_t n) {
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms computed (index 1 based) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure handled as 0 */
        cache[1] = 1;                    /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend cache if needed */
    if ((size_t)n > cache_len) {
        /* Ensure capacity */
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure handled as 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cand = prev + 1;
            while (true) {
                uint64_t cand64 = (uint64_t)cand;
                if (!is_prime_uint64(cand64)) {
                    uint64_t prev64 = (uint64_t)prev;
                    if (gcd_u64(prev64, cand64) == 1) {
                        break;
                    }
                }
                cand++;
            }
            cache[i] = cand;
        }
        cache_len = n;
    }

    return cache[n];
}
