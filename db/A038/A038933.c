/*
 * A038933  Primes p such that 55 is a square mod p.
 * https://oeis.org/A038933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038933: primes p for which 55 is a quadratic residue modulo p */
#include <stdbool.h>
#include <stdlib.h>
/* utilities for primality testing and modular exponentiation */
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    /* Cache of primes that satisfy the condition */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_start = 2;        /* next candidate to test for primality */

    while (cache_size < n) {
        /* Find the next prime ≥ next_start */
        idx_t p = next_start;
        for (;; ++p)
            if (is_prime(p))
                break;
        next_start = p + 1;            /* next search will start after this prime */

        /* Check if 55 is a quadratic residue modulo p.
           If p | 55 (p == 5 or 11), we treat it as a residue. */
        bool ok;
        if (p == 2 || p == 5 || p == 11) {
            ok = true;
        } else {
            /* Jacobi symbol (55|p) is 1 iff 55^((p-1)/2) ≡ 1 (mod p) */
            uint64_t leg = powmod64(55ULL, ((uint64_t)p - 1) >> 1, (uint64_t)p);
            ok = (leg == 1);
        }

        if (ok) {
            /* Grow the cache if necessary */
            if (cache_cap <= cache_size) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_size + 1)
                    new_cap = cache_size + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
