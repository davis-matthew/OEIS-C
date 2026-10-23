/*
 * A154679  Primes p such that the sum of digits of p+2 is a composite number.
 * https://oeis.org/A154679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5075, u32 n <= 5076, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154679: Primes p such that the sum of digits of p+2 is a composite number.
   Code implements a direct sieve-like search, storing already found terms in a
   static cache.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Provided helper prototypes */
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache holding the sequence values */
    static seq_t *cache = NULL;        /* array of terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* current capacity of cache */
    static uint64_t last_checked = 1;  /* last candidate prime examined */

    /* Ensure the cache is large enough to contain the nth term */
    while (cache_len < (size_t)n) {
        /* Find the next candidate prime after last_checked */
        uint64_t p = last_checked + 1;
        if (p <= 2)          p = 2;
        if (p > 2 && (p & 1) == 0) p++;      /* make odd if >2 */

        /* Search for the next qualifying prime */
        while (true) {
            /* Check primality of p */
            if (is_prime_big((seq_t)p)) {
                /* Compute sum of digits of p+2 */
                seq_t sum = digit_sum((seq_t)(p + 2));

                /* Requires a composite sum (sum > 1 and not prime) */
                if (sum > (seq_t)1 && !is_prime_big(sum)) {
                    /* Grow cache if needed */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                      new_cap * sizeof(seq_t));
                        if (!tmp) abort();            /* allocation failed */
                        cache = tmp;
                        cache_cap = new_cap;
                    }

                    /* Store the prime */
                    cache[cache_len++] = (seq_t)p;
                    last_checked = p;
                    break;                              /* next term found */
                }
            }

            /* Step to the next candidate, keeping it odd */
            p++;
            if (p > 2 && (p & 1) == 0) p++;
        }
    }

    return cache[(size_t)n - 1];
}
