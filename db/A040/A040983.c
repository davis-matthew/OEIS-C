/*
 * A040983  Primes p such that x^5 = 19 has no solution mod p.
 * https://oeis.org/A040983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that x^5 = 19 has no solution mod p.
   A prime p is included if
     p ≡ 1 (mod 5)   and   19^((p-1)/5) mod p ≠ 1.
   5 itself is excluded because 19 is a fifth‑power residue mod 5. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;   /* number of elements stored */
    static size_t cacheCap  = 0;   /* allocated capacity */
    static uint64_t cand = 2;      /* next integer to test for primality */

    /* Expand cache until it contains at least n terms */
    while (cacheSize < (size_t)n) {
        /* find next prime */
        while (!is_prime(cand)) cand++;
        uint64_t p = cand;
        cand++;                      /* advance for next iteration */

        /* 5 is a trivial solution; skip it */
        if (p == 5) continue;

        /* Only primes ≡ 1 mod 5 may fail the 5‑th‑power test */
        if (p % 5 != 1) continue;

        /* Exponent for the test */
        uint64_t exp = (p - 1) / 5;

        /* Compute 19^exp (mod p) */
        seq_t val = mod_pow((seq_t)19, exp, (seq_t)p);

        if (val != (seq_t)1) {
            /* store the prime in the cache */
            if (cacheSize == cacheCap) {
                size_t newCap = cacheCap ? cacheCap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cacheCap = newCap;
            }
            cache[cacheSize++] = (seq_t)p;
        }
    }

    /* cache[0] = a(1) */
    return cache[n - 1];
}
