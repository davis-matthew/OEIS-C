/*
 * A215281  Primes congruent to {2, 3, 4} mod 17.
 * https://oeis.org/A215281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes ≡ {2, 3, 4} (mod 17)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 993 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined offset behaviour */

    /* Static storage for generated data */
    static seq_t *cache = NULL;          /* prime values satisfying the condition */
    static size_t cache_cnt = 0;         /* number of cached values */
    static size_t cache_cap = 0;         /* capacity of the cache array */

    static seq_t *primes = NULL;         /* all primes discovered (for trial division) */
    static size_t primes_cnt = 0;        /* number of primes stored */
    static size_t primes_cap = 0;        /* capacity of the primes array */

    static seq_t candidate = 5;          /* next candidate to test for primality */
    static bool initialized = false;     /* initialization flag */

    /* First‑time initialisation */
    if (!initialized) {
        initialized = true;

        /* initialise cache array */
        cache_cap = 4;
        cache = mem_new(seq_t, cache_cap);

        /* initialise primes array */
        primes_cap = 4;
        primes = mem_new(seq_t, primes_cap);

        /* seed the lists with the first two primes */
        primes[primes_cnt++] = 2;
        primes[primes_cnt++] = 3;

        /* 2 and 3 satisfy the congruence condition */
        cache[cache_cnt++] = 2;
        cache[cache_cnt++] = 3;
    }

    /* Generate new primes until we have at least n values */
    while (cache_cnt < n) {
        bool is_prime = true;
        for (idx_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            seq_t pp = p * p;
            if (pp > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store the prime */
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 4;
                primes = mem_resize(seq_t, primes, primes_cap);
            }
            primes[primes_cnt++] = candidate;

            /* test the congruence condition */
            seq_t r = candidate % 17;
            if (r == 2 || r == 3 || r == 4) {
                if (cache_cnt == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_cnt++] = candidate;
            }
        }

        candidate += 2;   /* skip even numbers (candidate always odd) */
    }

    return cache[n - 1];
}
