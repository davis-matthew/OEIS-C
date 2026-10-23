/*
 * A186279  Numbers k such that the sum of the digits of k plus the sum of the
 *          digits of k^2 plus the sum of the digits of k^3 is prime.
 * https://oeis.org/A186279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A186279:  a(n) = the nth integer k such that
    sum of digits of k + sum of digits of k^2 + sum of digits of k^3
    is a prime number. */
/*  Implementation: search all integers starting at 1 and
    store the discovered terms in a growing cache.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* last candidate checked */
    static idx_t last_k  = 0;

    /* number of terms currently stored */
    static idx_t cached_terms = 0;

    /* return cached value if already found */
    if (n <= cached_terms) {
        return cache[n - 1];
    }

    /* extend the cache until we have at least n terms */
    while (cached_terms < n) {
        /* next integer to test */
        ++last_k;
        seq_t k = (seq_t)last_k;

        /* compute digit sums of k, k^2 and k^3 */
        seq_t s1 = digit_sum(k);
        seq_t s2 = digit_sum(big_pow(k, (seq_t)2));
        seq_t s3 = digit_sum(big_pow(k, (seq_t)3));
        seq_t sum = s1 + s2 + s3;

        /* keep the term if the sum is prime */
        if (is_prime_big(sum)) {
            /* grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
            ++cached_terms;
        }
    }

    return cache[n - 1];
}
