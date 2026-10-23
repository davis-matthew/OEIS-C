/*
 * A124065  Numbers k such that 8*k - 1 and 8*k + 1 are twin primes.
 * https://oeis.org/A124065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers k such that 8*k-1 and 8*k+1 are twin primes.
   The function computes the n-th such k by searching successive
   integer values of k.  Results are memoised so that calls in any
   order remain efficient. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0) {             /* offset 1; value for n==0 is undefined */
        return 0;
    }

    /* memoisation store */
    static seq_t *cache = NULL;     /* array of found k's */
    static size_t cache_count = 0;  /* number of entries stored */
    static size_t cache_cap = 0;    /* capacity of cache buffer */
    static idx_t next_k = 1;        /* next candidate k to test */

    /* extend cache until it contains at least n elements */
    while (cache_count < n) {
        idx_t k = next_k;
        /* test for twin primes 8*k-1 and 8*k+1 */
        if (is_prime(k * 8 - 1ULL) && is_prime(k * 8 + 1ULL)) {
            /* grow cache if needed */
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count] = (seq_t)k;
            cache_count++;
        }
        next_k++;                      /* check next integer k */
    }

    return cache[n - 1];
}
