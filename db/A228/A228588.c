/*
 * A228588  Starting from a(1)=1, a(n) is the minimum integer greater than
 *          a(n-1) such that a(n)+a(n-1) and a(n)*a(n-1)+1 are both primes.
 * https://oeis.org/A228588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1)=1, for n>1 a(n) is the least integer >a(n-1) such that both
 *            a(n)+a(n-1) and a(n)*a(n-1)+1 are prime
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0)  /* undefined, return 0 to avoid UB */
        return (seq_t)0;

    /* initialise cache with a(1)=1 */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;  /* a(1) */
        cache_len = 1;
    }

    /* extend cache if required */
    if ((size_t)n > cache_len) {
        size_t target = (size_t)n;
        /* grow cache capacity (doubling) */
        while (cache_cap < target) cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms */
        for (size_t i = cache_len + 1; i <= target; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t cur  = prev + (seq_t)1;        /* start with opposite parity */

            /* search for the minimal admissible integer */
            for (;;) {
                seq_t sum = cur + prev;
                seq_t prod = cur * prev + (seq_t)1;

                if (is_prime_big(sum) && is_prime_big(prod))
                    break;
                cur += (seq_t)2;                /* keep parity */
            }
            cache[i - 1] = cur;  /* store a(i) */
        }
        cache_len = target;
    }

    return cache[n - 1];
}
