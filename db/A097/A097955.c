/*
 * A097955  Primes p such that p divides 5^((p-1)/2) - 2^((p-1)/2).
 * https://oeis.org/A097955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 3242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: primes p such that p divides 5^((p-1)/2) - 2^((p-1)/2)
 * (offset 1, i.e. a(1) = 3)
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/next_prime.h" /* uint64_t next_prime(uint64_t start) */

/* sequence entry */
seq_t a(idx_t n)
{
    /* static cache for previously found terms (1‑based) */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next prime to test, starts at 3 */
    static uint64_t next_p = 3;
    static bool initialized = false;

    /* initialise cache on first entry */
    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_p   = 3;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = next_p;                     /* current candidate prime */
        seq_t   prime_seq = (seq_t)p;

        /* exponent (p-1)/2 fits in 64 bits for all primes we encounter */
        uint64_t exp = (p - 1ULL) >> 1;

        seq_t pow5 = mod_pow((seq_t)5, exp, prime_seq);
        seq_t pow2 = mod_pow((seq_t)2, exp, prime_seq);

        if (pow5 == pow2) {
            if (cache_len == cache_cap) {        /* grow cache if needed */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = prime_seq;
        }

        next_p = next_prime(p);                 /* move to next prime */
    }

    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
