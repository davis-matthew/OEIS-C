/*
 * A049552  Primes p such that x^20 = 2 has a solution mod p.
 * https://oeis.org/A049552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of all primes p such that the congruence
 *  x^20 ≡ 2 (mod p) has a solution.  For an odd prime p this
 *  holds iff 2^((p-1)/gcd(20,p-1)) ≡ 1 (mod p).  The trivial case
 *  p = 2 is also included.  The function a(n) returns the n‑th such
 *  prime, with a(1) = 2.  The implementation keeps a cache of known
 *  terms and extends it on demand using a simple trial–division
 *  prime generator.*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

/* Euclidean gcd for 64‑bit unsigned integers */

/* Check whether a prime p satisfies the 20‑th power residue condition. */
static inline bool ok_prime(uint64_t p) {
    if (p == 2) return true;               /* 0^20 ≡ 2 (mod 2) */
    uint64_t g = gcd_u64(20ULL, p - 1ULL);
    uint64_t e = (p - 1ULL) / g;
    return mod_pow((seq_t)2, e, (seq_t)p) == (seq_t)1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Static cache of results and auxiliary data */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    static uint64_t *prime_list   = NULL;
    static size_t   prime_len     = 0;
    static size_t   prime_cap     = 0;

    static uint64_t candidate = 2;   /* next odd number to test (1 → 2, 3, 5, …) */

    /* Ensure enough capacity for the cache */
    if (n > cache_size) {
        /* Initialize prime list capacity if necessary */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        }

        while (cache_size < (size_t)n) {
            bool is_prime = true;

            if (candidate > 2) {
                /* Test divisibility by known primes up to √candidate */
                double sq = sqrt((double)candidate);
                uint64_t limit = (uint64_t)sq + 1;
                for (size_t i = 0; i < prime_len && prime_list[i] <= limit; ++i) {
                    if (candidate % prime_list[i] == 0) {
                        is_prime = false;
                        break;
                    }
                }
            }

            if (is_prime) {
                /* Store new prime in list */
                if (prime_len == prime_cap) {
                    prime_cap <<= 1;
                    prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_len++] = candidate;

                /* If it satisfies the condition, cache it */
                if (ok_prime(candidate)) {
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap << 1 : 8;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_size++] = (seq_t) candidate;
                }
            }

            /* Advance to the next candidate */
            if (candidate == 2) {
                candidate = 3;
            } else {
                candidate += 2;  /* only odd numbers beyond 2 */
            }
        }
    }

    return cache[(size_t)n - 1];
}
