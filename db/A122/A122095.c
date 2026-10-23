/*
 * A122095  Primes p for which 8*p+1 divides 2^p-1.
 * https://oeis.org/A122095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th prime p for which 8p+1 divides 2^p-1.
 * The function searches successive primes and tests the congruence
 * 2^p ≡ 1 (mod 8p+1) using fast modular exponentiation.
 *
 * Facilities used:
 *   - is_prime(idx_t)          from ../utils/is_prime.h
 *   - mul64(uint64_t, uint64_t, uint64_t) from ../utils/mul64.h
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mul64.h"

/* fast binary exponentiation modulo mod (all 64‑bit values) */
static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul64(result, base, mod);
        base = mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1 – behaviour for n=0 is irrelevant */
        return 0;

    /* cached list of found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* the next integer to test as a prime candidate */
    static idx_t candidate = 2;

    while (terms_len < n) {
        /* advance to the next prime */
        while (true) {
            if (is_prime(candidate))
                break;
            candidate++;
            if (candidate > 2 && !(candidate & 1))
                candidate++;              /* skip even numbers */
        }

        /* check the property 2^p ≡ 1 (mod 8p+1) */
        uint64_t p   = (uint64_t)candidate;
        uint64_t mod = 8ULL * p + 1ULL;
        if (mod_pow_u64(2, p, mod) == 1ULL) {
            /* store the prime in the cache */
            if (terms_len + 1 > terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (terms ? (seq_t *)realloc(terms,
                            terms_cap * sizeof(seq_t))
                              : (seq_t *)malloc(terms_cap * sizeof(seq_t)));
                if (!terms)
                    return 0;            /* allocation failure – give up */
            }
            terms[terms_len++] = (seq_t)p;
        }

        /* move to the next candidate for the next iteration */
        candidate++;
        if (candidate > 2 && !(candidate & 1))
            candidate++;
    }

    return terms[n - 1];
}
