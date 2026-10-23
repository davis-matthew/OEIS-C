/*
 * A136153  Composites one larger than a prime, with exactly four distinct
 *          prime factors.
 * https://oeis.org/A136153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   computed by searching primes p; if p+1 has 4 distinct prime
 *            factors, include p+1
 * Range:     u8 none, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count_distinct_primes, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static state */
    static seq_t *cache   = NULL;
    static size_t  cache_cnt = 0;
    static size_t  cache_cap = 0;
    static uint64_t cur_prime = 1;      /* prime to start from */

    /* allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* generate terms until we have n of them */
    while (cache_cnt < (size_t)n) {
        cur_prime = next_prime(cur_prime);          /* next prime after cur_prime */
        seq_t cand = (seq_t)(cur_prime + 1);

        /* check distinct prime factor count */
        if (count_distinct_primes((uint64_t)cand) == 4) {
            /* grow cache if needed */
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
            }
            cache[cache_cnt++] = cand;
        }
    }

    return cache[n - 1];
}
