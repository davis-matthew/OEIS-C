/*
 * A181704  Numbers m=2^(t-1)*(2^t-5), where 2^t-5 is prime.
 * https://oeis.org/A181704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code computes A181704: a(n) = 2^(t-1) * (2^t-5) where t is the nth integer
 * such that 2^t-5 is prime. The function scans t upward, tests 
 * primality of 2^t-5 using is_prime_big, and memoizes found terms. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The smallest t that has been examined so far. t starts from 3. */
    static idx_t last_t = 2;

    /* Expand cache to accommodate n terms */
    while (cache_len < n) {
        /* Find next t that makes 2^t-5 prime */
        do {
            ++last_t;
            /* 2^t */
            seq_t pow_t = pow2(last_t);
            /* 2^t - 5 */
            seq_t pminus5 = pow_t - (seq_t)5;
            /* test primality */
            if (is_prime_big(pminus5)) {
                /* 2^(t-1) */
                seq_t pow_t_minus1 = pow2(last_t - 1);
                /* term = 2^(t-1) * (2^t - 5) */
                seq_t term = pow_t_minus1 * pminus5;

                /* Grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                    if (new_cache == NULL)
                        abort(); /* allocation failure */
                    if (cache)
                        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = term;
                break;
            }
        } while (1);
    }

    return cache[n - 1];
}
