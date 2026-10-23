/*
 * A142213  Primes congruent to 16 mod 41.
 * https://oeis.org/A142213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 16 (mod 41)
 * Range:     u8 n <= 1, u16 n <= 164, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure the cache contains at least n terms */
    if (n > cache_len) {
        size_t needed = n;
        if (needed > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }
        /* Generate new terms until we have n of them */
        seq_t cand;
        if (cache_len == 0)
            cand = (seq_t)16;        /* start from the smallest 16 (mod 41) */
        else
            cand = cache[cache_len - 1] + (seq_t)41;
        while (cache_len < needed) {
            if (is_prime_big(cand))
                cache[cache_len++] = cand;
            cand += (seq_t)41;
        }
    }

    return cache[n - 1];
}
