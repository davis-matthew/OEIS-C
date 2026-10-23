/*
 * A118491  Product of first n Chen primes.
 * https://oeis.org/A118491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n Chen primes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Chen prime: prime p for which p+2 is prime or a product of two primes) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"   /* smallest prime factor */

/* Helper: test whether a prime p is a Chen prime */
static bool is_chen_prime(uint64_t p)
{
    /* p is guaranteed prime when called */
    uint64_t q = p + 2;
    if (is_prime((idx_t)q))
        return true;                 /* p+2 is prime */

    /* otherwise q must be a product of two primes */
    uint64_t s = _smallest_prime_factor(q);   /* smallest prime factor of q */
    if (s == 0 || s == q)                /* q is prime or 1 – not a semiprime */
        return false;
    uint64_t r = q / s;                  /* second factor */
    return is_prime((idx_t)r);           /* check that second factor is prime */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;          /* cache[0] = 1, cache[ k ] = product of first k Chen primes */
    static idx_t cache_len = 0;          /* currently computed up to index cache_len-1 */
    static idx_t cache_cap = 0;

    /* static state for generating primes */
    static uint64_t next_candidate = 1;  /* candidate for next prime to test */
    static uint64_t last_chen_prime = 0;/* last found Chen prime (unused after multiply) */

    /* Ensure element 0 is available (a(0)=1) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;     /* allocation failure, result undefined */
        cache[0] = 1;             /* a(0) = 1 */
        cache_len = 1;
    }

    /* Extend cache if requested index not yet computed */
    while (n >= cache_len) {
        /* Find next prime p */
        do {
            ++next_candidate;
        } while (!is_prime((idx_t)next_candidate));

        /* Check if this prime is a Chen prime */
        if (!is_chen_prime(next_candidate))
            continue; /* skip until we find a Chen prime */

        /* We need space for the new term */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 1;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!cache) return 0;   /* allocation failure, result undefined */
        }

        /* Multiply previous product by this Chen prime */
        cache[cache_len] = cache[cache_len - 1] * (seq_t)next_candidate;
        ++cache_len;
    }

    return cache[(idx_t)n];
}
