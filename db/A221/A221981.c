/*
 * A221981  Primes q = 4*p+1, where p == 2 (mod 5) is also prime.
 * https://oeis.org/A221981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 87, u32 n <= 60, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of primes p \u2260 2 (mod 5) with q = 4*p+1 prime.
   a(n) = 4*p + 1 where p is a prime ≡ 2 (mod 5) and 4*p + 1 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility function protos */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start); */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(seq_t n);    */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n);      */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for indices before the offset */

    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* last p examined; start with 1 so that next_prime gives 2 */
    static uint64_t last_p    = 1;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* get next prime p */
        uint64_t p = next_prime(last_p);
        last_p = p;

        /* test the special congruence condition */
        if ((p % 5U) == 2U) {
            /* compute q = 4*p + 1 */
            seq_t q = (seq_t)4 * (seq_t)p + (seq_t)1;

            /* q must also be prime */
            if (is_prime_big(q)) {
                /* extend cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failed */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = q;
            }
        }
        /* next prime will be found on the next loop iteration */
    }

    return cache[n - 1];
}
