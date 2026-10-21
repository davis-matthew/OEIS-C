/*
 * A067954  Primes related to the nondecreasing subsequence of A007605 (sums
 *          of digits of primes).
 * https://oeis.org/A067954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=2; a(n+1) is the smallest prime with digit sum >= sum of
 *            digits of a(n)
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cache of computed values */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t cache_len = 0;          /* number of values stored in cache */
static seq_t last_prime;             /* last prime computed (a(n)) */
static seq_t last_sum;               /* sum of digits of last_prime */

/* Ensure that cache contains values up to index n (1‑based) */
static void cache_ensure(idx_t n)
{
    while (cache_len < n) {
        if (cache_len == 0) {            /* initialize first term */
            last_prime = 2;
            last_sum   = 2;
            if (cache_cap == 0) {
                cache_cap = 32;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            cache[0] = last_prime;
            ++cache_len;
            continue;
        }

        /* Find next prime with sum of digits >= last_sum */
        seq_t candidate = last_prime + 1;
        while (true) {
            if (is_prime_big(candidate)) {
                seq_t s = digit_sum(candidate);
                if (s >= last_sum) {
                    last_prime = candidate;
                    last_sum   = s;
                    break;
                }
            }
            ++candidate;
        }

        /* Grow cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[cache_len++] = last_prime;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 (behavior irrelevant) */
    cache_ensure(n);
    return cache[n - 1];
}
