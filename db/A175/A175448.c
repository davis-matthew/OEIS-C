/*
 * A175448  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=15.
 * https://oeis.org/A175448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined by
   a(1) = 2, and for n > 1
   a(n) = the smallest prime > a(n-1) such that
   a(n) + a(n-1) ≡ 0 (mod 15) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of computed terms: indices 1 .. cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0, cache_cap = 0;

    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Initialise cache with the first term a(1) = 2 */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 1;
        cache[0] = (seq_t)2;
    }

    /* Grow cache and compute missing terms up to n */
    if (n > cache_len) {
        if (n > cache_cap) {
            cache_cap = n * 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* last computed term a(i-1) */
            idx_t prev = (idx_t)cache[cache_len - 1];
            /* residue that candidate must satisfy */
            idx_t target = (15 - (prev % 15)) % 15;
            idx_t cand = prev + 1;
            for (;; ++cand) {
                if ((cand % 15) == target && is_prime(cand))
                    break;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
