/*
 * A152625  Prime numbers p such that 2p+1, 4p+1, 8p+1, 10p+1, 14p+1 and 16p+1
 *          are all composite numbers.
 * https://oeis.org/A152625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that 2p+1, 4p+1, 8p+1, 10p+1, 14p+1 and
 *            16p+1 are all composite numbers
 * Range:     u8 none, u16 n <= 212, u32 n <= 212, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"  /* assumes existence of is_prime_big(seq_t) */

/* External function required by the build */
seq_t a(idx_t n)
{
    /* CACHE: store found sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* PRIMES: store all primes found so far for trial division */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;

    /* Helper to grow prime array */
    if (primes_cap == 0) {
        primes_cap = 8;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* Ensure cache has enough capacity */
    if (cache_len == 0 && cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate sequence terms until we have the nth one */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate */
        static uint64_t cand = 2;   /* start with 2, will be incremented below */
        /* Move to next potential prime */
        if (cand == 2) {
            cand = 3;
        } else {
            cand += 2;  /* only test odd numbers */
        }

        /* Test cand for primality using trial division by existing primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (!is_prime) continue;

        /* Record the prime for future tests */
        if (primes_len == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_len++] = cand;

        /* Check the required composite conditions */
        seq_t p = (seq_t)cand;

        seq_t e1 = 2 * p + 1;
        seq_t e2 = 4 * p + 1;
        seq_t e3 = 8 * p + 1;
        seq_t e4 = 10 * p + 1;
        seq_t e5 = 14 * p + 1;
        seq_t e6 = 16 * p + 1;

        if (is_prime_big(e1)) continue;
        if (is_prime_big(e2)) continue;
        if (is_prime_big(e3)) continue;
        if (is_prime_big(e4)) continue;
        if (is_prime_big(e5)) continue;
        if (is_prime_big(e6)) continue;

        /* All six numbers are composite: store in cache */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = p;
    }

    /* Return the requested term */
    return cache[n - 1];
}
