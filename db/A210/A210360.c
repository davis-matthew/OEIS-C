/*
 * A210360  Prime numbers p such that x^2 + x + p produces primes for x = 0..1
 *          but not x = 2.
 * https://oeis.org/A210360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 564, u32 n <= 630, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: list of primes p such that p is prime,
   p+2 is also prime, and p+6 is composite (not prime). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cache_len = 0;    /* number of cached terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_candidate = 2; /* next integer to test for being a prime */

    /* return cached value if already computed */
    if (n > 0 && (size_t)(n - 1) < cache_len)
        return cache[n - 1];

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        seq_t p = next_candidate;

        /* find the next qualifying prime */
        for (;;) {
            if (is_prime_big(p)) {
                seq_t p2 = p + (seq_t)2;
                if (is_prime_big(p2)) {
                    seq_t p6 = p + (seq_t)6;
                    if (!is_prime_big(p6)) {
                        /* satisfy all conditions: store p */
                        if (cache_len == cache_cap) {
                            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                            seq_t *tmp = (seq_t *)realloc(cache,
                                                          new_cap * sizeof(seq_t));
                            /* assume realloc succeeds */
                            cache = tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_len++] = p;
                        next_candidate = p + (seq_t)1; /* start from next integer next time */
                        break; /* exit inner loop: found one term */
                    }
                }
            }
            /* move to next integer */
            p = p + (seq_t)1;
            next_candidate = p; /* keep next_candidate updated for next outer run */
        }
    }

    return cache[n - 1];
}
