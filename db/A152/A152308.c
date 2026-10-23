/*
 * A152308  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/8.
 * https://oeis.org/A152308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 241, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152308: Primes p such that the multiplicative order of 2 mod p is (p-1)/8 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* static state to cache already found terms */
    static seq_t *cache = NULL;    /* array of primes that satisfy the condition */
    static size_t   cache_size = 0;  /* number of primes stored */
    static size_t   cache_cap   = 0;  /* allocated capacity of cache */
    static uint64_t next_candidate = 73;  /* next prime to test */

    /* Grow cache until we have n terms */
    while (cache_size < n) {
        uint64_t p = next_candidate;

        /* Only primes that are 1 mod 8 can have ord_p(2) = (p-1)/8,
           because (p-1)/8 must be an integer, hence p ≡ 1 (mod 8). */
        if ((p & 7ULL) == 1ULL) {
            uint64_t t = (p - 1ULL) / 8ULL;          /* the required order */
            seq_t mod_p = (seq_t)p;

            /* Check that 2^t ≡ 1 (mod p) */
            if (mod_pow((seq_t)2, t, mod_p) == (seq_t)1) {
                /* factor t and verify minimality of the order */
                uint64_t primes[32];
                uint64_t exps[32];
                size_t   pf_cnt = 0;
                factor_u64(t, primes, exps, &pf_cnt);

                bool ok = true;
                for (size_t i = 0; i < pf_cnt && ok; ++i) {
                    uint64_t q = primes[i];
                    if (mod_pow((seq_t)2, t / q, mod_p) == (seq_t)1)
                        ok = false;
                }

                if (ok) {
                    /* Store the prime in the cache */
                    if (cache_size >= cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) { /* allocation failure */
                            /* In an actual implementation we would handle this,
                               but for brevity we simply return 0. */
                            return (seq_t)0;
                        }
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_size++] = (seq_t)p;
                }
            }
        }

        /* Prepare the next candidate for the next iteration */
        next_candidate = next_prime(p);
    }

    /* nth term is at index n-1 (OEIS offset starts at 1) */
    return cache[n - 1];
}
