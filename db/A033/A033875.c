/*
 * A033875  Skipping from prime to prime by least powers of 2.
 * https://oeis.org/A033875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n+1) = a(n) + 2^k where k minimal such that a(n)+2^k
 *            is prime
 * Range:     u8 n <= 18, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"        /* seq_t pow2(idx_t exp); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;           /* cache[0] holds a(1) */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Convert idx_t to size_t for array indexing; safe for n <= 10000 */
    size_t target = (size_t)n;
    if (target == 0) return (seq_t)0; /* undefined, but avoid crash */

    /* Grow cache if necessary */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)2;               /* a(1) = 2 */
    }

    while (cache_len < target) {
        seq_t cur = cache[cache_len - 1];
        seq_t nxt = 0;
        for (idx_t k = 0;; ++k) {
            seq_t candidate = cur + pow2(k);
            if (is_prime_big(candidate)) {
                nxt = candidate;
                break;
            }
        }
        /* Grow array if needed */
        if (cache_len == cache_cap) {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = nxt;
    }

    return cache[target - 1];
}
