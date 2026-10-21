/*
 * A224870  Numbers m such that m^2 + (m+3)^2 is prime.
 * https://oeis.org/A224870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the n-th integer m such that m^2 + (m+3)^2 is prime. */
/* Sequence order follows OEIS offset: a(1) = 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_m = 1;
    static size_t found   = 0;

    if (n == 0)
        return 0; /* undefined, but avoid crashes */

    while (found < n) {
        seq_t val = 2 * next_m * next_m + 6 * next_m + 9;
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return 0; /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_m;
            ++found;
        }
        ++next_m;
    }

    return cache[n - 1];
}
