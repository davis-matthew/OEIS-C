/*
 * A175122  a(1)=3. a(n) = smallest prime > a(n-1) such that a(n)-a(n-1) =
 *          2*m, where m is composite.
 * https://oeis.org/A175122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A175122:
   a(1) = 3
   a(n) = smallest prime > a(n-1) such that (a(n)-a(n-1))/2 is composite
   (so a(n)-a(n-1) = 2*m,  m composite) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache to memoise computed values */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;
    static idx_t   cache_cap = 0;

    if (n == 0)               /* offset 1; undefined for n<1, return 0 */
        return (seq_t)0;

    /* initialise cache if first call */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* ensure cache contains terms up to index n */
    while (cache_len < n) {
        seq_t prev;

        /* a(1) is fixed to 3 */
        if (cache_len == 0) {
            prev = (seq_t)3;
            cache[cache_len++] = prev;
            continue;          /* next loop will compute a(2) */
        }

        prev = cache[cache_len - 1];

        seq_t candidate = prev;
        seq_t next_val = 0;

        for (;;) {
            /* find next prime strictly larger than candidate */
            if ((uint64_t)candidate <= UINT64_MAX - 1) {
                uint64_t pj = next_prime((uint64_t)candidate);
                next_val = (seq_t)pj;
            } else {
                /* fallback: increment until prime (unlikely for n<=10000) */
                seq_t cand = candidate + (seq_t)1;
                while (!is_prime_big(cand))
                    cand = cand + (seq_t)1;
                next_val = cand;
            }

            seq_t diff = next_val - prev;        /* >0 */
            seq_t m    = diff / (seq_t)2;        /* diff is even for odd primes */

            if (m > (seq_t)1 && !is_prime_big(m))
                break;                           /* suitable next term */

            candidate = next_val;                /* test next prime */
        }

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
        }

        cache[cache_len++] = next_val;
    }

    return cache[n - 1];
}
