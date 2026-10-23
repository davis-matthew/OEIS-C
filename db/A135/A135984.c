/*
 * A135984  a(n) = 24(prime(n))+7.
 * https://oeis.org/A135984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 * prime(n) + 7 (prime(n) = nth prime, n≥1)
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square‑root: return ⌊√x⌋ for 0 ≤ x < 2^63 */
static inline idx_t integer_sqrt(idx_t x) {
    idx_t r = 0;
    /* For the range of x we ever encounter a simple linear search is fast
       enough and avoids any floating point or library calls. */
    while ((r + 1) * (r + 1) <= x) {
        r++;
    }
    return r;
}

/* Static cache of primes */
static uint64_t *prime_cache = NULL;
static idx_t prime_cache_count = 0;
static size_t prime_cache_cap = 0;

/* Expand the cache to hold at least 'need' primes */
static void ensure_primes(idx_t need) {
    if (need <= prime_cache_count) {
        return;
    }

    /* Grow the capacity if required */
    if (prime_cache == NULL) {
        prime_cache_cap = 512;
        prime_cache = (uint64_t *)malloc(prime_cache_cap * sizeof(uint64_t));
        prime_cache_count = 0;
    } else if ((size_t)need > prime_cache_cap) {
        while ((size_t)need > prime_cache_cap) {
            prime_cache_cap <<= 1;
        }
        prime_cache = (uint64_t *)realloc(prime_cache, prime_cache_cap * sizeof(uint64_t));
    }

    /* Start searching for primes from the first candidate after the last
       known prime.  If we have no primes yet, start at 2. */
    uint64_t candidate = (prime_cache_count == 0) ? 2 : prime_cache[prime_cache_count - 1] + 1;

    while (prime_cache_count < need) {
        /* Test candidate for primality by trial division with known primes. */
        idx_t limit = integer_sqrt(candidate);
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cache_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p > limit) {
                break;
            }
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            prime_cache[prime_cache_count++] = candidate;
        }
        candidate++;
    }
}

/* Main sequence function as required by the framework */
seq_t a(idx_t n) {
    if (n == 0) {
        /* For n = 0 the OEIS offset does not define a value – return 0. */
        return (seq_t)0;
    }

    ensure_primes(n);

    uint64_t p = prime_cache[n - 1];
    /* The calculation fits comfortably in 64 bits for all n tested. */
    return p * 24 + 7;
}
