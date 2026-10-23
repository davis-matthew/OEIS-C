/*
 * A142387  Primes congruent to 36 mod 47.
 * https://oeis.org/A142387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   This function returns the n‑th prime congruent to 36 modulo 47,
 *            * obtained by scanning the arithmetic progression 47*k+36 and
 *            testing each * candidate for primality
 * Range:     u8 n <= 1, u16 n <= 142, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* offset starts at 1; 0 is unused */
    if (n == 0)
        return 0;

    /* Static cache of previously computed primes */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;

    /* Extend cache until we have at least n primes */
    if (cache_len < n) {
        if (cache == NULL) {
            cache_cap = 10;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache_len = 0;
        }

        /* Determine the starting index k for candidates */
        idx_t k_start;
        if (cache_len == 0) {
            k_start = 1;                 /* 47*1 + 36 = 83 is the first term */
        } else {
            seq_t last = cache[cache_len - 1];
            seq_t k_val = (last - (seq_t)36) / (seq_t)47;
            k_start = (idx_t)k_val + 1;
        }

        /* Generate primes of the desired form until we have n */
        for (idx_t k = k_start; cache_len < n; ++k) {
            seq_t cand = ((seq_t)47 * (seq_t)k) + (seq_t)36;
            if (is_prime_big(cand)) {
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    cache = tmp;
                }
                cache[cache_len++] = cand;
            }
        }
    }

    return cache[n - 1];
}
