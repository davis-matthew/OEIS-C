/*
 * A173092  Numbers k such that 3k-4, 3k-2, 3k+2, and 3k+4 are primes.
 * https://oeis.org/A173092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 28, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k such that 3k-4, 3k-2, 3k+2, 3k+4 are all prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_k = 3;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)3;   /* a(1) = 3 */
        cache_len = 1;
        next_k = 5;            /* start with 5, odd numbers only */
        initialized = true;
    }

    while (cache_len < n) {
        /* only odd k > 3 */
        seq_t k = next_k;
        next_k += 2;

        seq_t t  = (seq_t)3 * k;   /* 3k */
        seq_t p1 = t - 4;
        seq_t p2 = t - 2;
        seq_t p3 = t + 2;
        seq_t p4 = t + 4;

        if (is_prime_big(p1) && is_prime_big(p2) &&
            is_prime_big(p3) && is_prime_big(p4)) {

            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
