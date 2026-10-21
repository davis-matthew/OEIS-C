/*
 * A070916  a(1)=1, a(n) is the smallest integer >= a(n-1) such that
 *          a(n)*a(n-1) - 1 is prime.
 * https://oeis.org/A070916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k ≥ a(n‑1) such that k*a(n‑1)‑1 is prime. a(1) =
 *            1
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 62, u64 n <= 1000,
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
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) {
        /* undefined for 0, but return 0 to keep type */
        return (seq_t)0;
    }

    /* Ensure cache[0] = 1 (a(1)) */
    if (cached_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cached_len = 1;
    }

    /* Extend cache up to n */
    while (cached_len < n) {
        seq_t prev = cache[cached_len - 1];
        seq_t cand = prev;                 /* start from previous value */

        /* Find smallest candidate such that cand*prev‑1 is prime */
        while (true) {
            seq_t prod = cand * prev - (seq_t)1;
            if (is_prime_big(prod))
                break;
            ++cand;
        }

        /* Grow cache if necessary */
        if (cached_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure: abort */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cached_len++] = cand;
    }

    return cache[n - 1];
}
