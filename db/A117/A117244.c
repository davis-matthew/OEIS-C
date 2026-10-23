/*
 * A117244  Single (or isolated or non-twin) primes (A007510) that are not
 *          Chen primes (A109611).
 * https://oeis.org/A117244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 2839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117244:
   Prime p such that
     (p-2) and (p+2) are both composite
     AND (p+2) is neither prime nor a semiprime
   i.e. p is an isolated (single) prime and not a Chen prime.
   a(1) = 79, a(2) = 97, ... */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms   = NULL;
    static size_t terms_sz = 0;
    static size_t terms_cap = 0;

    /* Next integer to test for primality. */
    static uint64_t cand = 2;

    /* Grow the cache until it contains at least n terms. */
    while (terms_sz < (size_t)n) {
        uint64_t p = 0; /* Current prime candidate. */

        /* Find the next prime. */
        while (true) {
            if (cand <= 2) {
                p = cand;               /* 2 */
                cand = 3;               /* next odd candidate */
                break;
            }
            if (is_prime(cand)) {
                p = cand;
                cand += 2;              /* skip even numbers */
                break;
            }
            cand += 2;
        }

        /* Check that p is an isolated prime: neither neighbor is prime. */
        if (p > 2 && is_prime(p - 2))
            continue;                    /* p is the larger twin of (p-2) */

        /* p+2 must not be prime. */
        uint64_t np2 = p + 2;
        if (is_prime(np2))
            continue;

        /* p+2 must not be a semiprime: total exponent sum != 2. */
        uint64_t primes_arr[64];
        uint64_t exps_arr[64];
        size_t   cnt = 0;
        factor_u64(np2, primes_arr, exps_arr, &cnt);

        size_t total_factors = 0;
        for (size_t i = 0; i < cnt; ++i)
            total_factors += exps_arr[i];

        if (total_factors == 2)
            continue;                    /* semiprime */

        /* p satisfies all conditions; store it. */
        if (terms_sz == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 16;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_sz++] = (seq_t)p;
    }

    return terms[(size_t)n - 1];
}
