/*
 * A097363  Positive integers n such that 2n-13 is prime.
 * https://oeis.org/A097363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n such that 2*n-13 is prime
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Persistent cache for already found terms */
    static idx_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 7;   /* value before the first admissible n */

    while (cache_len < n) {
        idx_t candidate = last_checked + 1;
        if (candidate < 8) candidate = 8;   /* first admissible term */

        for (/* candidate already set */ ; ; ++candidate) {
            if (isPrime(2 * candidate - 13)) {
                /* extend cache if necessary */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 64;
                    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                last_checked = candidate;
                break;
            }
        }
    }

    return (seq_t)cache[n - 1];
}
