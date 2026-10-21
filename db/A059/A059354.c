/*
 * A059354  Primes p such that x^27 = 2 has no solution mod p, but x^9 = 2 has
 *          a solution mod p.
 * https://oeis.org/A059354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p > 2 for which * 2 is a 9th‑power residue
 *            modulo p (existence of x: x^9 ≡ 2 (mod p)) * but 2 is NOT a
 *            27th‑power residue modulo p. * The function generates such
 *            primes on demand, caching them for * repeated calls. Only the
 *            standard integer type utilities are used
 * Range:     u8 none, u16 n <= 29, u32 n <= 107, u64 n <= 107, u128 n <= 107,
 *            big any n
 * Helpers:   isPrime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"   /* bool isPrime(idx_t x); */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* local 64‑bit gcd (different name from the library) */
static inline uint64_t gcd64_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)      /* idx_t is an unsigned 64‑bit type, seq_t is the value type */
{
    /* cache: 1‑based array of primes that satisfy the condition */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;
    static idx_t cache_cap = 0;

    /* next odd number to test for primality */
    static uint64_t cand = 3;

    while (cache_sz < n) {
        /* find the next prime */
        while (!isPrime(cand))
            cand += 2;                      /* skip even numbers */

        uint64_t p = cand;                  /* current prime candidate */

        /* compute the gcds needed for the exponent tests */
        uint64_t g9  = gcd64_uint64(p - 1ULL, 9ULL);
        uint64_t g27 = gcd64_uint64(p - 1ULL, 27ULL);

        /* exponents to use with modular exponentiation */
        uint64_t e9  = (p - 1ULL) / g9;
        uint64_t e27 = (p - 1ULL) / g27;

        /* test the two power‑residue conditions */
        seq_t pow9  = mod_pow((seq_t)2ULL, e9, (seq_t)p);
        seq_t pow27 = mod_pow((seq_t)2ULL, e27, (seq_t)p);

        if (pow9 == (seq_t)1ULL && pow27 != (seq_t)1ULL) {
            /* store the prime in the cache */
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)p;
        }

        cand += 2;  /* proceed to the next odd candidate */
    }

    return cache[n - 1];
}
