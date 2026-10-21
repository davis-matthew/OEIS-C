/*
 * A061237  Prime numbers == 1 (mod 9).
 * https://oeis.org/A061237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 1 (mod 9)
 * Range:     u8 n <= 8, u16 n <= 1079, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static cache for found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;   /* number of terms in cache */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */

    if (n == 0) return 0;          /* undefined below the offset */

    /* grow the cache until it contains at least n terms */
    while (cache_cnt < n) {
        idx_t candidate;
        if (cache_cnt == 0) {
            candidate = 19;                /* first term */
        } else {
            candidate = (idx_t)cache[cache_cnt - 1] + 1;
        }

        /* search for the next prime ≡ 1 (mod 9) */
        for (;;) {
            if (candidate % 9 == 1 && is_prime(candidate)) {
                /* enlarge cache if needed */
                if (cache_cnt >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_ptr = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                    if (!new_ptr)        /* allocation failure */
                        return 0;
                    cache     = new_ptr;
                    cache_cap = new_cap;
                }
                cache[cache_cnt++] = (seq_t)candidate;
                break;
            }
            ++candidate;
        }
    }

    return cache[n - 1];
}
