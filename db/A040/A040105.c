/*
 * A040105  Primes p such that x^4 = 5 has a solution mod p.
 * https://oeis.org/A040105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040105 – primes p for which x^4 = 5 has a solution mod p.
 *  Algorithm: generate primes in increasing order, test the
 *  congruence conditions, and store the qualifying primes.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t) */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* static cache of primes that satisfy the condition */
    static uint64_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_checked = 0; /* last prime inspected */

    /* return cached value if it already exists */
    if (n <= cache_used)
        return (seq_t)cache[n - 1];

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }

    /* find primes until we have n terms */
    while (cache_used < n) {
        /* fetch the next prime */
        last_checked = next_prime(last_checked + 1);
        uint64_t p = last_checked;

        bool ok = false;
        if (p == 2 || p == 5)
            ok = true;
        else if (p % 4 == 3) {
            /* primes ≡ 11 or 19 (mod 20) */
            if (p % 20 == 11 || p % 20 == 19)
                ok = true;
        } else { /* p % 4 == 1 */
            seq_t res = mod_pow((seq_t)5, (p - 1) / 4, (seq_t)p);
            if (res == (seq_t)1)
                ok = true;
        }

        if (ok) {
            /* grow cache if necessary */
            if (cache_used == cache_cap) {
                cache_cap <<= 1;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_used++] = p;
        }
    }

    return (seq_t)cache[n - 1];
}
